#ifndef ALGORITHM_LIST_HPP
#define ALGORITHM_LIST_HPP

#include<boost/shared_ptr.hpp>
#include<string>
#include<map>

namespace test
{
    class algorithm_wrapper_base
    {
        public:
        virtual ~algorithm_wrapper_base() {}
        virtual algorithm_wrapper_base* clone() const = 0;

        virtual void apply() = 0;
    };

    template<class A>
    class algorithm_wrapper : public algorithm_wrapper_base
    {
        public:
        algorithm_wrapper(A& algorithm) : impl_(algorithm) {}
        algorithm_wrapper_base* clone() const
        {
            return new algorithm_wrapper<A>(*this);
        }

        void apply() { impl_.apply(); }

        private:
        A& impl_;
    };

    class algorithm_dict
    {
        public:
        typedef boost::shared_ptr<algorithm_wrapper_base> algorithm_type;
        typedef std::map< std::string, algorithm_type > container_type;

        template<class A>
        void add(const std::string& name, A& algorithm)
        {
            boost::shared_ptr< algorithm_wrapper_base > wrapped(new algorithm_wrapper<A>(algorithm));
            algorithms_[name] = wrapped;
        }

        const algorithm_wrapper_base& operator[] (const std::string& x) 
        { 
            return *algorithms_[x]; 
        }

        container_type::iterator begin() { return algorithms_.begin(); }
        container_type::iterator end() { return algorithms_.end(); }
        //const algorithm_wrapper_base& operator[] (const std::string& x) const
        //{ 
        //    return *algorithms_[x]; 
        //}

        private:
        container_type algorithms_;
    };

}

#endif
