#ifndef FEATURE_LIST_HPP
#define FEATURE_LIST_HPP

#include<boost/shared_ptr.hpp>
#include<string>
#include<map>

namespace test
{
    class feature_wrapper_base
    {
        public:
        virtual ~feature_wrapper_base() {}
        virtual feature_wrapper_base* clone() const = 0;

        virtual void parse(std::string block) = 0;
        virtual void update() = 0;
        virtual void print(std::ostream& os) = 0;
        virtual void register_algorithms() = 0;
        template<class F> typename F::parameter_type* param()
        {
            void* param = get_param();
            return static_cast<typename F::parameter_type*>(param);
        }
        virtual void* get_param() = 0;

    };

    template<class F>
    class feature_wrapper : public feature_wrapper_base
    {
        public:
        typedef F feature_type;
        feature_wrapper(F& feature) : impl_(feature) {}
        feature_wrapper_base* clone() const
        {
            return new feature_wrapper<F>(*this);
        }

        void parse(std::string block) { impl_.parse(block); }
        void update() { impl_.update(); }
        void print(std::ostream& os) { impl_.print(os); }
        void register_algorithms() { impl_.register_algorithms(); }
        void* get_param() 
        { 
            return impl_.get_param(); 
        }

        private:
        F impl_;
    };

    class feature_dict
    {
        public:
        typedef boost::shared_ptr<feature_wrapper_base> feature_type;
        typedef std::map< std::string, feature_type >
            container_type;

        feature_dict() : features_() {}

        template<class F>
        void add(const std::string& name, F& feature)
        {
            boost::shared_ptr< feature_wrapper_base > wrapped(new feature_wrapper<F>(feature));
            features_[name] = wrapped;
        }

        feature_wrapper_base& operator[] (const std::string& x) 
        { 
            return *features_[x]; 
        }

        container_type::iterator begin() { return features_.begin(); }
        container_type::iterator end() { return features_.end(); }
        //const feature_wrapper_base& operator[] (const std::string& x) const
        //{ 
        //    return *features_[x]; 
        //}

        private:
        container_type features_;
    };

    std::ostream& operator<< (std::ostream& os, feature_wrapper_base& feature)
    {
        feature.print(os);
        return os;
    }
}

#endif
