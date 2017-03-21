#ifndef PROLOGTRANSLATIONSTACK_H
#define PROLOGTRANSLATIONSTACK_H

#define ADD_STR "+"
#define SUBS_STR "-"
#define MULT_STR "*"
#define DIV_STR "//"
#define AND_STR "#/\\"
#define OR_STR "#\\/"
#define MOD_STR "rem"
#define ABS_LEFT_STR "abs("
#define ABS_RIGHT_STR ")"
#define DOMAIN_LEFT ""
#define DOMAIN_RIGHT ""
#define DOMAIN_MIDDLE ".."
#define DOMAIN_JOIN "\\/"
#define DOMAIN_EQ "in"
#define NOT_EQUALS_STR "#\\="
#define EQUALS_STR "#="
#define BIGGER_STR "#>"
#define LESSER_STR "#<"
#define BIGGER_EQ_STR "#>="
#define LESSER_EQ_STR "#=<"

#include <stack>
#include <string>
#include <sstream>
#include <set>

#include <QString>
#include <QFile>
#include <QTemporaryFile>
#include <QTextStream>

#include <fluidicmachinemodel/constraintssolverinterface/translationstack.h>
#include <fluidicmachinemodel/machine_graph_utils/variablenominator.h>
#include <fluidicmachinemodel/rules/conjunction.h>
#include <fluidicmachinemodel/rules/arithmetic/binaryoperation.h>
#include <fluidicmachinemodel/rules/arithmetic/unaryoperation.h>
#include <fluidicmachinemodel/rules/equality.h>

#include "constraintengine/prologexecutor.h"

#include "constraintengine/constraintsenginelibrary_global.h"

/**
 * @brief The PrologTranslationStack class translates a set of abstract rules to a prolog predicate.
 *
 * The PrologTranslationStack class implements the TranslationStack interface at the FluidicMachineModel library, translate an abstract
 * set of rules to a prolog file containing a predicate that the swi-prolog interpreter can use to solve the constraint problem specified
 * by the rules.
 *
 * Internally a Pushdown automaton is used to translate the rules, each trasnlated rules is represented as a string.
 *
 * @sa TranslationStack
 */
class PROLOGTRANSLATIONSTACK_EXPORT PrologTranslationStack : public TranslationStack
{
public:
    /**
     * @brief PrologTranslationStack creates a new stack
     */
    PrologTranslationStack();
    /**
     * @brief ~PrologTranslationStack destroys the stack
     */
    virtual ~PrologTranslationStack();

    /**
     * @brief pop removes the top of the stack
     */
    virtual void pop();
    /**
     * @brief clears the stack
     */
    virtual void clear();
    /**
     * @brief addHeadToRestrictions adds a new string to the actualrestriction vector
     *
     * addHeadToRestrictions gets the string at the top of the stack, adds it to the actualrestriction vector,
     * and remove it from the stack.
     */
    virtual void addHeadToRestrictions();
    /**
     * @brief stackVariable adds to the top of the stack a new string with the name of a variable, also adds this
     * variable to the variable table.
     *
     * @param name name of the variable to be stack
     */
    virtual void stackVariable(const std::string & name);
    /**
     * @brief stackNumber adds to the top of the stack a new string with an integer value
     * @param value integer value to be stacked.
     */
    virtual void stackNumber(int value);
    /**
     * @brief stackArithmeticBinaryOperation removes the top of the stack and the string below it,
     * creates a new string that contains both strings join with the corresponding numerical
     * operator(+,-,*,...etc). Adds this new created string to the top of the stack.
     *
     * @param arithmeticOp integer value that represents the numerical operation used, this operations
     * are available in BinaryOperation::BinaryOperator from the class BinaryOperation of the
     * FluidicMachineModel library.
     *
     * @sa BinaryOperation
     */
    virtual void stackArithmeticBinaryOperation(int arithmeticOp);
    /**
     * @brief stackArithmeticUnaryOperation removes the string at the top of the stack, creates a new
     * string that contains: the just removed string and the corresponding unary operation(absolute value...etc).
     * Adds the newly created string to the top of the stack.
     *
     * @param unaryOp integer value that represents the unary operation to be applied, these operations are avialable
     * in UnaryOperation::UnaryOperators from the UnaryOperation class of the FluidicMachineModel library.
     *
     * @sa UnaryOperation
     */
    virtual void stackArithmeticUnaryOperation(int unaryOp);
    /**
     * @brief stackEquality removes two string from the top of the stack, creates a new string that contains: the top
     * string the corresponding comparation operation(=,<,>,!=...) and the string below the top. Adds this new string to
     * the top of the stack.
     *
     * @param op integer value that represents the comparation operation to be applied. The avialable comparation operations are
     * available in Equality::ComparatorOp from the Equality class of the FluidicMachineModel library.
     *
     * @sa Equality
     */
    virtual void stackEquality(int op);
    /**
     * @brief stackBooleanConjuction removes two string from the top of the stack, creates a new string that contains: the top
     * string, the corresponding boolean operation(and,or,...) and the string below the top. Adds this new string to
     * the top of the stack.
     *
     * @param booleanOp op integer value that represents the boolean operation to be applied. The avialable boolean operations are
     * available in BinaryOperation::BinaryOperators from the BinaryOperation class of the FluidicMachineModel library.
     */
    virtual void stackBooleanConjuction(int booleanOp);
    /**
     * @brief stackImplication deprecated, to be removed
     */
    virtual void stackImplication();
    /**
     * @brief stackVarDomain creates a new variable domain for the given stack.
     *
     * stackVarDomain removes the top of the stack makes a new string that has the top of the stack and "in" token. The size of the stack must be
     * even if not an error string is added to the top of the stack. A new string is made that contains all the variables of the stack in pairs of two
     * joined by the token ".." between each member of a pair and with the token "\/" between each pair.
     *
     * For example for this stack [P_1,-2,-1,1,2] the method creates a stack like this: [P_1 in -2..-1 \/ 1..2].
     */
    virtual void stackVarDomain();

    /**
     * @brief getRoutingEngine creates a new PrologExecutor.
     * @return a pointer to the newly created PrologExecutor
     *
     * @sa PrologExecutor
     */
    virtual RoutingEngine* getRoutingEngine();

    /**
     * @brief generateMethodHeather generates the Head of the prolog rule with the variables in the varTable.
     * @return a string containing the head of the rule.
     *
     * @sa varTable
     */
    std::string generateMethodHeather();
    /**
     * @brief generateLabelingFoot generates a labeling instruction at the end of the prolog rule body. This instruction is necesary by the clpfd library
     * to minimize the number of pumps and valves used to mantain a set of flows.
     *
     * @return a string containing the minimization intsruction.
     *
     * @sa http://www.swi-prolog.org/pldoc/man?predicate=labeling/2
     */
    std::string generateLabelingFoot();
    inline const std::vector<std::string> & getTranslatedRestriction() const {
        return actualRestriction;
    }
    /**
     * @brief getVarTable returns the varTable
     * @return a constant reference to the varTable attribute.
     *
     * @sa varTable
     */
    inline const std::set<std::string> & getVarTable() const {
        return varTable;
    }

 protected:
    /**
     * @brief stack string stack that is used to translate the abstract rules
     */
    std::stack<std::string> stack;
    /**
     * @brief actualRestriction vector of strings with all the rules translated, this vector is fill when the method addHeadToRestrictions() is invoked.
     *
     * @sa addHeadToRestrictions()
     */
    std::vector<std::string> actualRestriction;
    /**
     * @brief varTable set of strings that contains all the variables used in the rules.
     */
    std::set<std::string> varTable;

    /**
     * @brief opToStr returns the string that match the corresponding arithmetic operation.
     *
     * @param op enum type that represents the atithmetic operation to translate. This operations
     * are available in BinaryOperation::BinaryOperator from the class BinaryOperation of the
     * FluidicMachineModel library.
     * @return string that represents the corresponding operation.
     */
    std::string opToStr(BinaryOperation::BinaryOperators op);
    /**
     * @brief boolOpToStr returns the string that match the corresponding boolean operation.
     *
     * @param op enum type that represents the boolean operation to translate. This operations
     * are available in Conjunction::BoolOperators from the class Conjunction of the
     * FluidicMachineModel library.
     * @return string that represents the corresponding operation.
     */
    std::string boolOpToStr(Conjunction::BoolOperators op);
    /**
     * @brief unaryOpToStr returns a pair of string that represenst the corresponding unary operation.
     *
     * unaryOpToStr returns a pair of string that represenst the corresponding binary operation. For example
     * if the operation to be translated is an absolute value the pair of string will be <"|","|"> beacuse to do
     * the absolute value of something both strings need to wrap it.
     *
     * @param op enum type that represents the boolean operation to translate. This operations
     * are available in UnaryOperation::UnaryOperators from the class UnaryOperation of the
     * FluidicMachineModel library.
     *
     * @return pair of strings that wraps the operand and represents the corresponding unary operation.
     */
    std::tuple<std::string,std::string> unaryOpToStr(UnaryOperation::UnaryOperators op);
    /**
     * @brief equalityOPtoStr returns the string that match the corresponding comparation operation.
     *
     * @param op enum type that represents the comparation operation to translate. This operations
     * are available in Equality::ComparatorOp from the class Equality of the FluidicMachineModel library.
     * @return string that represents the corresponding operation.
     */
    std::string equalityOPtoStr(Equality::ComparatorOp op);
    /**
     * @brief tabulateString puts the correct tabulation in a string, this is just for better visualition of a file.
     * @param str string to be tabulated.
     * @return the string with the correspònding tabulators.
     */
    std::string tabulateString(const std::string & str);
};

#endif // PROLOGTRANSLATIONSTACK_H
