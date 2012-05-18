#ifndef TEST_FEATURE_HPP
#define TEST_FEATURE_HPP

#include "../feature_list.hpp"
#include "../algorithm_list.hpp"

#include <boost/array.hpp>
#include <boost/mpl/bool.hpp>
#include <cmath>
#include <iostream>
#include <iomanip>

namespace test
{
    typedef boost::array<double, 2> cpar;

    struct test_feature_parameter
    {
        test_feature_parameter() : modus(1), mass(1) 
        {
            //std::cout << "constructing param" << std::endl;
            value[0] = value[1] = 0.0;
            velocity[0] = velocity[1] = 0.0;
            force[0] = force[1] = 0.0;
        }
        //test_feature_parameter(const test_feature_parameter& other) :
        //    modus(other.modus), value(other.value), velocity(other.velocity),
        //    force(other.force), mass(other.mass) {}
        void update()
        {
            value[0]    = value[1];
            velocity[0] = velocity[1];
            force[0]    = force[1];
        }

        void print(std::ostream& os)
        {
            os 
                << std::setw(15) << "modus:"     << std::setw(8) << modus << std::endl
                << std::setw(15) << "mass:"      << std::setw(8) << mass << std::endl
                << std::setw(15) << "value:"     << std::setw(8) << value[1] << std::endl
                << std::setw(15) << "velocity:"  << std::setw(8) << velocity[1] << std::endl
                << std::setw(15) << "force:"     << std::setw(8) << force[1] << std::endl;
        }

        int         modus;
        cpar        value, velocity, force;
        double      mass;
    };

    template<class FL, class P>
    struct update_value_algorithm
    {
        update_value_algorithm(FL& feature, P& param)
            : features(features), param(param) {}

        inline void apply()
        {
            //const double dt = features["core"].param<core>.time_step;
            const double dt = 0.1;
            param.value[1] = param.value[0] + dt * param.velocity[1];

            //std::cout << "update value:" << std::endl;
            //param.print(std::cout);
        }

        FL& features;
        P&  param;
    };

    template<class FL, class P>
    struct update_velocity_algorithm
    {
        update_velocity_algorithm(FL& feature, P& param)
            : features(features), param(param) {}

        inline void apply()
        {
            //const double dt = features["core"].param<core>.time_step;
            const double dt = 0.1;
            param.velocity[1] = param.velocity[0] + dt * param.force[1] / param.mass;
        }

        FL& features;
        P&  param;
    };

    template<class FL, class P>
    struct update_force_algorithm
    {
        update_force_algorithm(FL& feature, P& param)
            : features(features), param(param) {}

        inline void apply()
        {
            //const double dt = features["core"].param<core>.time_step;
            const double dt = 0.1;
            param.force[1] = 0.1;//std::sin(dt);
        }

        FL& features;
        P&  param;
    };

    class test_feature
    {
        public:
        typedef test_feature_parameter parameter_type;

        test_feature(feature_dict& features, algorithm_dict& algorithms) : 
            param(),
            update_value(features, param),
            update_velocity(features, param),
            update_force(features, param),
            features(features),
            algorithms(algorithms)
        {}
        test_feature(const test_feature& other) :
            enabled(other.enabled),
            param(other.param),
            features(other.features),
            algorithms(other.algorithms),
            update_value(other.features, param),
            update_velocity(other.features, param),
            update_force(other.features, param)
        {}

        void parse(std::string block) {}
        void update() {param.update();}
        parameter_type* get_param() { return &param; }
        void print(std::ostream& os)
        {
            os << "Test Feature: " << std::endl;
            param.print(os);
            os << "/Test Feature" << std::endl;
        }
        void register_algorithms()
        {
            algorithms.add("update test feature value", update_value);
            algorithms.add("update test feature velocity", update_velocity);
            algorithms.add("update test feature force", update_force);
        }

        friend struct update_value_algorithm<feature_dict, parameter_type>;
        friend struct update_velocity_algorithm<feature_dict, parameter_type>;
        friend struct update_force_algorithm<feature_dict, parameter_type>;

        private:
        bool            enabled;
        parameter_type  param;

        update_value_algorithm<feature_dict, parameter_type>      update_value;
        update_velocity_algorithm<feature_dict, parameter_type>   update_velocity;
        update_force_algorithm<feature_dict, parameter_type>      update_force;

        feature_dict&   features;
        algorithm_dict& algorithms;

        public:
        static std::string name;
    };

    std::string test_feature::name = "test_feature";

}


#endif 
