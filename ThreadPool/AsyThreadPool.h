#include<queue>
#include<mutex>
#include<vector>
#include<thread>
#include<functional>
#include<future>

template<typename T>
class SafeQueue{
private:
    std::queue<T> q_;
    std::mutex mutex_;
public:
    SafeQueue(){}
    ~SafeQueue(){}
    bool empty(){
        std::unique_lock<std::mutex> lock(mutex_);
        return q_.empty();
    }
    void enqueue(T& t){
        std::unique_lock<std::mutex> lock(mutex_);
        q_.push(t);
    }
    T dequeue(){
        std::unique_lock<std::mutex> lock(mutex_);
        T t=q_.front();
        q_.pop();
        return t;    
    }
};

class ThreadPool{
private:
    bool stop_;
    std::vector<std::thread> workers_;
    SafeQueue<std::function<void()>> safeQueue_;
    std::mutex mutex_;
    std::condition_variable cv_;
public:
    ThreadPool(int num):workers_(std::vector<std::thread>(num)),stop_(false){

    }

    ~ThreadPool(){
        
    }

    void Shutdown(){
        {
            std::unique_lock<std::mutex> lock(mutex_);
            stop_=true;
        }
        cv_.notify_all();
        for(auto& worker:workers_){
            worker.join();
        }
    }

    void Init(){
        for(int i=0;i<workers_.size();i++){
            workers_[i]=std::thread([this](){
                for(;;){
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(this->mutex_);
                        this->cv_.wait(lock,  [this](){ 
                            return this->stop_ || !this->safeQueue_.empty();
                        });

                        if(this->stop_){
                            return;
                        }
                        // 虚假唤醒
                        if(this->safeQueue_.empty()){ 
                            continue;
                        }
                        task=this->safeQueue_.dequeue();
                    }
                    task();
                }
            });
        }
    }
    template<typename F,typename...Args>
    auto Submit(F&& f,Args&&... args) -> std::future<decltype(f(args...))>{
        std::function<decltype(f(args...))()> functors=std::bind(std::forward<F>(f),std::forward<Args>(args)...);
        auto taskPtr=std::make_shared<std::packaged_task<decltype(f(args...))()>>(functors);
        std::function<void()> warpper_func=[taskPtr](){
            (*taskPtr)();
        };

        safeQueue_.enqueue(warpper_func);
        cv_.notify_one();
        return taskPtr->get_future();
    }
};