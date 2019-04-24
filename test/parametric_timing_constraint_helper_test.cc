/*
 * @author Masaki Waga
 * @date 2019-01-28
 */

#include <sstream>
#include "automaton_parser.hh"
#include <boost/lexical_cast.hpp>
#include <boost/test/unit_test.hpp>
#include "../src/parametric_timing_constraint_helper.hh"

BOOST_AUTO_TEST_SUITE(ParametricTimingConstraintHelperTest)
  BOOST_AUTO_TEST_SUITE(LexicalCastTest)
    using boost::lexical_cast;

    BOOST_AUTO_TEST_CASE(le) {
      std::string str = "x0 < 3";
      const auto result = lexical_cast<ParametricTimingConstraintHelper>(str);
    }

    BOOST_AUTO_TEST_CASE(vectorLe) {
      std::string str = "{x0 < 3}";
      const auto result = lexical_cast<std::vector<ParametricTimingConstraintHelper>>(str);
    }

  BOOST_AUTO_TEST_SUITE_END()
  BOOST_AUTO_TEST_SUITE(ParseTest)

    BOOST_AUTO_TEST_CASE(complicatedGE) {
      std::stringstream stream;
      stream << "x0 + p1 >= x1 - 10 + p0";
      ParametricTimingConstraintHelper helper;
      stream >> helper;
      BOOST_CHECK_EQUAL(std::get<std::size_t>(helper.head.front().first), 0);
      BOOST_CHECK_EQUAL(helper.head.front().second, ParametricTimingConstraintHelper::kind_t::VARIABLE);
      BOOST_CHECK_EQUAL(helper.tail.front().size(), 1);
      BOOST_CHECK_EQUAL(helper.tail.front().front().first, ParametricTimingConstraintHelper::op_t::PLUS);
      BOOST_CHECK_EQUAL(helper.tail.front().front().second.second,
                        ParametricTimingConstraintHelper::kind_t::PARAMETER);
      BOOST_CHECK_EQUAL(std::get<std::size_t>(helper.tail.front().front().second.first), 1);

      BOOST_CHECK_EQUAL(helper.comparison, ParametricTimingConstraintHelper::comparison_t::GE);

      BOOST_CHECK_EQUAL(std::get<std::size_t>(helper.head.back().first), 1);
      BOOST_CHECK_EQUAL(helper.head.back().second, ParametricTimingConstraintHelper::kind_t::VARIABLE);
      BOOST_CHECK_EQUAL(helper.tail.back().size(), 2);
      BOOST_CHECK_EQUAL(helper.tail.back().front().first, ParametricTimingConstraintHelper::op_t::MINUS);
      BOOST_CHECK_EQUAL(helper.tail.back().front().second.second,
                        ParametricTimingConstraintHelper::kind_t::CONSTANT);
      BOOST_CHECK_EQUAL(std::get<Parma_Polyhedra_Library::Coefficient>(helper.tail.back().front().second.first), 10);
      BOOST_CHECK_EQUAL(helper.tail.back().back().first, ParametricTimingConstraintHelper::op_t::PLUS);
      BOOST_CHECK_EQUAL(helper.tail.back().back().second.second,
                        ParametricTimingConstraintHelper::kind_t::PARAMETER);
      BOOST_CHECK_EQUAL(std::get<std::size_t>(helper.tail.back().back().second.first), 0);
    }

  BOOST_AUTO_TEST_SUITE_END()
  BOOST_AUTO_TEST_SUITE(ExtractTest)

    BOOST_AUTO_TEST_CASE(complicatedGE) {
      using namespace Parma_Polyhedra_Library;
      std::stringstream stream;
      stream << "x0 + p1 >= x1 - 10 + p0";
      ParametricTimingConstraintHelper helper;
      stream >> helper;

      Constraint constraint;
      helper.extract(2, constraint);
      Constraint expectedConstraint = Variable(2) + Variable(1) >= Variable(3) - 10 + Variable(0);
      BOOST_TEST(constraint.is_equal_to(expectedConstraint));
    }

  BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END() // ParametricTimingConstraintHelperTest