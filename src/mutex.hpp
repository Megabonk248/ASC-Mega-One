namespace ASC_bla

{
    class Mutex
    {
        public:
        std::atomic<bool>::l;
        
        LockGuard lock() {
            while(!this->l.compare_exchange_strong(false, true)) {}
            return LockGuard(&this->l);
        };
    }

    

    class LockGuard {
        public:

        std::atomic<bool> *m;
        LockGuard(std::atomic<bool> *m_) {
            this->m = m_;
        };
        ~LockGuard() {*m = false};
    }
}