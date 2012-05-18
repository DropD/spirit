#include "simulation.hpp"
#include "feature_list.hpp"
#include "algorithm_list.hpp"
#include "features/test_feature.hpp"

#include <iostream>

int main (int argc, char const* argv[])
{
    using test::simulation;
    using test::test_feature;

    simulation md;
    md.add_feature<test_feature>();

    //test_feature::parameter_type& tpar = md.param<test_feature>();

    md.print();
    md.update();
    md.step();
    md.print();

    return 0;
}                       
