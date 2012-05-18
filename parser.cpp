#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/qi_match_attr.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/io.hpp>

#include <string>
#include <iostream>
#include <fstream>
#include <streambuf>

namespace parser
{
    namespace qi = boost::spirit::qi;
    namespace phoenix = boost::phoenix;
    namespace ascii = boost::spirit::ascii;

    struct param
    {
        bool enable;
    };

    struct nparam
    {
        std::string name;
        bool enable;
    };

    struct ultimate_parameter
    {
        std::string         name;
        bool                enable;
        std::vector<int>    ints;
        std::vector<double> doubles;
    };

}

BOOST_FUSION_ADAPT_STRUCT(
    parser::param,
    (bool, enable)
)

BOOST_FUSION_ADAPT_STRUCT(
    parser::nparam,
    (std::string, name)
    (bool, enable)
)

BOOST_FUSION_ADAPT_STRUCT(
    parser::ultimate_parameter,
    (std::string,           name)
    (bool,                  enable)
    (std::vector<int>,      ints)
    (std::vector<double>,   doubles)
)

namespace parser
{

    template<typename Iterator>
    struct upar_list_parser : qi::grammar<Iterator, std::vector<ultimate_parameter>(), ascii::space_type>
    {
        upar_list_parser() : upar_list_parser::base_type(start)
        {
            using ascii::upper;
            using ascii::space;
            using qi::eps;
            using qi::eol;
            using qi::char_;
            using qi::int_;
            using qi::double_;
            using qi::lit;
            using qi::lexeme;
            using qi::omit;

            name_       %= upper >> *upper;
            enable_     %= int_;
            ints_       %= lexeme[int_ % *space];
            doubles_    %= lexeme[double_ % *space];
            upar_       %= name_ >> -comments_ >> enable_ >> -comments_ >> -ints_ >> -comments_ >> -doubles_ >> -comments_ >> lit("END");
            comment_    %= lexeme[lit("#") >> *omit[((char_ | space) - eol)] >> eol];
            comments_   %= comment_ % eps;

            start %= -comments_ >> upar_ % (-comments_);

        }

        qi::rule<Iterator, std::string(), ascii::space_type>            name_;
        qi::rule<Iterator, bool(), ascii::space_type>                   enable_;
        qi::rule<Iterator, std::vector<int>(), ascii::space_type>       ints_;
        qi::rule<Iterator, std::vector<double>(), ascii::space_type>    doubles_;
        qi::rule<Iterator, ultimate_parameter(), ascii::space_type>     upar_;
        qi::rule<Iterator, ascii::space_type>                           comment_;
        qi::rule<Iterator, ascii::space_type>                           comments_;
        qi::rule<Iterator, std::vector<ultimate_parameter>(), ascii::space_type> start;
    };

    template<typename Iterator>
    struct block_parser : qi::grammar<Iterator, param(), ascii::space_type>
    {
        block_parser(std::string Name = "TEST") : block_parser::base_type(start)
        {
            using qi::int_;
            using qi::lit;
            using namespace qi::labels;
            //using qi::char_;
            //using qi::space;
            //using phoenix::push_back;

            mybool %= int_;
            test %= qi::eps >> lit(Name) >> mybool >> lit("END");
            start %=
                qi::eps
                >> test
                //>> lit(Name) >> int_ >> lit("END")
                //>> lit(Name) >> mybool >> lit("END")
                //>> test [push_back(phoenix::ref(v), _1)] % space
                ;
        }

        qi::rule<Iterator, bool(), ascii::space_type> test;
        qi::rule<Iterator, bool(), ascii::space_type> mybool;
        qi::rule<Iterator, param(), ascii::space_type> start;
    };

    template<typename Iterator>
    struct block_list_parser : qi::grammar<Iterator, std::vector<param>(), ascii::space_type>
    {
        block_list_parser(std::string Name = "TEST") : block_list_parser::base_type(start)
        {
            using qi::int_;
            using qi::lit;
            using qi::space;
            using namespace qi::labels;
            using phoenix::push_back;

            mybool %= int_;
            test   %= lit(Name) >> mybool >> lit("END");
            start  %= test % qi::eps;
        }

        qi::rule<Iterator, bool(), ascii::space_type> mybool;
        qi::rule<Iterator, param(), ascii::space_type> test;
        qi::rule<Iterator, std::vector<param>(), ascii::space_type> start;
    };

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

}

template<typename T>
std::ostream& operator<< (std::ostream& os, std::vector<T> tvec)
{
    os << "[";
    for (int i = 0; i < tvec.size(); ++i)
        os << tvec[i] << ", ";
    os << "]";
    return os;
}

int main (int argc, char const* argv[])
{
    std::cout << "/////////////////////////////////////////////////////////\n\n";
    std::cout << "\t\tA parameter parser for Spirit...\n\n";
    std::cout << "/////////////////////////////////////////////////////////\n\n";

    std::cout
        << "Format :\t"
        << "TEST int END"
        << std::endl;
    //std::cout << "Type [q or Q] to quit\n\n";

    using boost::spirit::ascii::space;
    typedef std::string::const_iterator iterator_type;
    typedef parser::block_parser<iterator_type> block_parser;
    typedef parser::block_list_parser<iterator_type> bl_parser;
    typedef parser::npar_list_parser<iterator_type> npl_parser;
    typedef parser::upar_list_parser<iterator_type> upl_parser;

    block_parser h; // My grammar
    bl_parser    k;
    npl_parser   l;
    upl_parser   u;
    std::string str;
    std::ifstream fin("test.par");

    //fin >> str;
    str.assign((std::istreambuf_iterator<char>(fin)), std::istreambuf_iterator<char>());
    std::cout << "parsing: " << str << std::endl;

    {
        //if (str.empty() || str[0] == 'q' || str[0] == 'Q')
        //    break;

        parser::param par;
        iterator_type iter = str.begin();
        iterator_type end = str.end();
        bool r;
        //r = phrase_parse(iter, end, h, space, par);

        std::vector<parser::param> pvec;
        iter = str.begin();
        end = str.end();
        //r = phrase_parse(iter, end, k, space, pvec);

        std::vector<parser::nparam> npvec;
        iter = str.begin();
        //r = phrase_parse(iter, end, l, space, npvec);
        //fin >> phrase_match(l, space, npvec);

        std::vector<parser::ultimate_parameter> upvec;
        iter = str.begin();
        r = phrase_parse(iter, end, u, space, upvec);

        if (r && iter == end)
        {
            std::cout << "-------------------------\n";
            std::cout << "Parsing succeeded\n";
            for(int i = 0; i < upvec.size(); ++i)
            {
                std::cout << boost::fusion::tuple_open('[');
                std::cout << boost::fusion::tuple_close(']');
                std::cout << boost::fusion::tuple_delimiter(", ");

                //std::cout << "got: " << boost::fusion::as_vector(upvec[i]) << std::endl;
                std::cout << "got: " << upvec[i].name << ": " << upvec[i].enable << ", " << std::endl
                            << "\t" << upvec[i].ints << ", " << std::endl
                            << "\t" << upvec[i].doubles << std::endl;
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

//int main (int argc, char const* argv[])
//{
//    std::cout << "/////////////////////////////////////////////////////////\n\n";
//    std::cout << "\t\tAn employee parser for Spirit...\n\n";
//    std::cout << "/////////////////////////////////////////////////////////\n\n";
//
//    std::cout
//        << "Give me an employee of the form :"
//        << "employee{age, \"surname\", \"forename\", salary } \n";
//    std::cout << "Type [q or Q] to quit\n\n";
//
//    using boost::spirit::ascii::space;
//    typedef std::string::const_iterator iterator_type;
//    typedef parser::block_parser<iterator_type> block_parser;
//
//    block_parser h; // My grammar
//    std::string str;
//    while (getline(std::cin, str))
//    {
//        if (str.empty() || str[0] == 'q' || str[0] == 'Q')
//            break;
//
//        parser::param par;
//        std::string::const_iterator iter = str.begin();
//        std::string::const_iterator end = str.end();
//        bool r = phrase_parse(iter, end, h, space, par);
//
//        if (r && iter == end)
//        {
//            std::cout << boost::fusion::tuple_open('[');
//            std::cout << boost::fusion::tuple_close(']');
//            std::cout << boost::fusion::tuple_delimiter(", ");
//
//            std::cout << "-------------------------\n";
//            std::cout << "Parsing succeeded\n";
//            std::cout << "got: " << boost::fusion::as_vector(par) << std::endl;
//            std::cout << "\n-------------------------\n";
//        }
//        else
//        {
//            std::cout << "-------------------------\n";
//            std::cout << "Parsing failed\n";
//            std::cout << "-------------------------\n";
//        }
//    }
//
//    std::cout << "Bye... :-) \n\n";
//    return 0;
//}
