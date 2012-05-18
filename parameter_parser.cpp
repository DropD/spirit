#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/io.hpp>
#include <boost/shared_ptr.hpp>

#include <string>
#include <iostream>
#include <fstream>
#include <streambuf>

namespace parser
{
    namespace qi = boost::spirit::qi;
    namespace phoenix = boost::phoenix;
    namespace ascii = boost::spirit::ascii;

    struct param1_
    {
        param1_() : enable(0) {}
        bool enable;
        double mass;
    };

    struct param2_
    {
        param2_() : enable(0) {}
        bool enable;
        int write_freq;
    };

    struct parameters
    {
        param1_ param1;
        param2_ param2;
    };

}

BOOST_FUSION_ADAPT_STRUCT(
    parser::param1_,
    (bool, enable)
    (double, mass)
)

BOOST_FUSION_ADAPT_STRUCT(
    parser::param2_,
    (bool, enable)
    (int, write_freq)
)

BOOST_FUSION_ADAPT_STRUCT(
    parser::parameters,
    (param1_, param1)
    (param2_, param2)
)

namespace parser
{

    //template<typename Iterator>
    //struct param_rules;
    //{
    //    static qi::rule<Iterator, param1_(), ascii::space_type>    param1_parser %= qi::lit("PARAM1") >> qi::int_ >> qi::double_ >> qi::lit("END");
    //    static qi::rule<Iterator, param2_(), ascii::space_type>    param2_parser %= qi::lit("PARAM1") >> qi::int_ >> qi::int_ >> qi::lit("END");
    //    static qi::rule<Iterator, parameters(), ascii::space_type> parameters_parser %= (param1_parser | param2_parser) % qi::eps;
    //};
    /*
    template<typename Iterator>
    struct npar_list_parser : qi::grammar<Iterator, std::vector<nparam>(), ascii::space_type>
    {
        npar_list_parser() : npar_list_parser::base_type(start)
        {
            using qi::int_;
            using qi::lit;
            using ascii::upper;
            using qi::eps;

            name    %= upper >> *upper;
            npar    %= name >> int_ >> lit("END");
                start   %= npar % qi::eps;
        }

        qi::rule<Iterator, std::string(), ascii::space_type> name;
        qi::rule<Iterator, nparam(), ascii::space_type> npar;
        qi::rule<Iterator, std::vector<nparam>(), ascii::space_type> start;
    };
    */

    //struct params_ : qi::symbols<char, param>
    //{
    //    params_()
    //    {
    //        add
    //            ("TEST"     , test_param())
    //            ;
    //    }
    //};

    template<typename Iterator>
    struct par_list_parser : qi::grammar<Iterator, std::vector<param>(), ascii::space_type>
    {
        par_list_parser() : par_list_parser::base_type(start)
        {
        }

        qi::rule<Iterator, std::vector<param>(), ascii::space_type> start;
    };

}

int main (int argc, char const* argv[])
{
    using boost::shared_ptr;

    std::cout
        << "Format :\t"
        << "NAME values ... END"
        << std::endl;

    using boost::spirit::ascii::space;
    typedef std::string::const_iterator iterator_type;
    typedef parser::par_list_parser<iterator_type> pl_parser;

    std::ifstream   fin("test2.par");
    std::string     str;

    //fin >> str;
    str.assign((std::istreambuf_iterator<char>(fin)), std::istreambuf_iterator<char>());
    std::cout << "parsing: " << str << std::endl;

    {

        std::vector<shared_ptr<parser::param> > pvec;
        iterator_type iter = str.begin();
        iterator_type end = str.end();
        bool r;
        //r = phrase_parse(iter, end, h, space, par);

        if (r && iter == end)
        {
            std::cout << "-------------------------\n";
            std::cout << "Parsing succeeded\n";
            for(int i = 0; i < pvec.size(); ++i)
            {
                std::cout << boost::fusion::tuple_open('[');
                std::cout << boost::fusion::tuple_close(']');
                std::cout << boost::fusion::tuple_delimiter(", ");

                std::cout << "got: " << boost::fusion::as_vector(pvec[i]) << std::endl;
            }
            std::cout << "\n-------------------------\n";
        }
        else
        {
            std::cout << "-------------------------\n";
            std::cout << "Parsing failed\n";
            std::cout << "-------------------------\n";
        }
    }

    std::cout << "Bye... :-) \n\n";
    return 0;
}
