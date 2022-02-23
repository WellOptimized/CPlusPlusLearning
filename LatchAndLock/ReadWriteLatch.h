#include <mutex>
#include <condition_variable>
#include <climits>


class ReaderWriterLatch{
    static const uint32_t MAX_READERS = UINT_MAX;
public:
    void WLock(){
        std::unique_lock<std::mutex> latch(mutex_);
        while(isWriting){
            reader_.wait(latch);
        }
        isWriting=true;
        while(reader_count_>0){
            writer_.wait(latch);
        }
    }
    void WUnlock(){
        std::lock_guard<std::mutex> guard(mutex_);
        isWriting=false;
        reader_.notify_all();
    }
    void RLock(){
        std::unique_lock<std::mutex> latch(mutex_);
        while(isWriting || reader_count_==MAX_READERS){
            reader_.wait(latch);
        }
        reader_count_++;
    }
    void RUnlock(){
        std::lock_guard<std::mutex> guard(mutex_);
        reader_count_--;
        if(isWriting){
            if(reader_count_==0){
                writer_.notify_one();
            }
        }else{
            if(reader_count_==MAX_READERS-1){
                reader_.notify_one();
            }
        }
    }
private:
    std::mutex mutex_;
    std::condition_variable writer_;
    std::condition_variable reader_;
    bool isWriting{false};
    uint32_t reader_count_{0};
};