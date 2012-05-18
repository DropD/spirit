#ifndef SIMULATION_HPP
#define SIMULATION_HPP

#include "feature_list.hpp"
#include "algorithm_list.hpp"

#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>

namespace test
{
    class simulation
    {
        public:
        simulation() : features(), algorithms() {}

        template<class F> void add_feature()
        {
            F feature(features, algorithms);
            features.add(F::name, feature);
            //std::cout << "added." << std::endl << features[F::name];
            features[F::name].register_algorithms();
            //std::cout << "added after register." << std::endl << features[F::name];
        }

        template<class F> typename F::parameter_type&
            param() 
        { 
            typename F::parameter_type& pref = *features[F::name].param<F>();
            return pref;
        }

        void update()
        {
            //using boost::lambda::_1;
            //using boost::lambda::bind;
            //std::for_each(features.begin(), features.end(), _1.second.update());
            typedef feature_dict::container_type::iterator itt;
            for(itt it = features.begin(), end = features.end(); it != end; ++it)
                (*it).second->update();
        }

        void step()
        {
            typedef algorithm_dict::container_type::iterator itt;
            for(itt it = algorithms.begin(), end = algorithms.end(); it != end; ++it)
                (*it).second->apply();
        }

        void print()
        {
            typedef feature_dict::container_type::iterator itt;
            for(itt it = features.begin(), end = features.end(); it != end; ++it)
                (*it).second->print(std::cout);
        }

        feature_dict    features;
        algorithm_dict  algorithms;
    };
}

#endif
