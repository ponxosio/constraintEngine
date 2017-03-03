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

#include <constraintssolverinterface/translationstack.h>
#include <machine_graph_utils/variablenominator.h>
#include <rules/conjunction.h>
#include <rules/arithmetic/binaryoperation.h>
#include <rules/arithmetic/unaryoperation.h>
#include <rules/equality.h>

#include "prologexecutor.h"

#include "constraintsenginelibrary_global.h"

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
     * @brief stackVariable adds to the top of the stack a new string with the name of a variable
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
     * operator(+,-,*,...etc). Adds this new created string to the top of the stack
     * @param arithmeticOp integer value that represents the numerical operation used, this operations
     * are available in BinaryOperation::BinaryOperator from the module BinaryOperation of the
     * FluidicMachineModel library.
     *
     * @sa BinaryOperation
     */
    virtual void stackArithmeticBinaryOperation(int arithmeticOp);
    /**
     * @brief stackArithmeticUnaryOperation removes the string at the top of the stack, creates a new
     * string that contains the just removed string and the corresponding unary operation(absolute value...etc).
     * Adds the newly created string to the top of the stack.
     *
     * @param unaryOp integer value that represents the unary operation to be applied, these operations are avialable
     * in UnaryOperation::UnaryOperators from the UnaryOperation module of the FluidicMachineModel library.
     *
     * @sa UnaryOperation
     */
    virtual void stackArithmeticUnaryOperation(int unaryOp);

    virtual void stackEquality(int op);
    virtual void stackBooleanConjuction(int booleanOp);
    virtual void stackImplication();
    virtual void stackVarDomain();

    virtual RoutingEngine* getRoutingEngine();

    std::string generateMethodHeather();
    std::string generateLabelingFoot();
    inline const std::vector<std::string> & getTranslatedRestriction () {
        return actualRestriction;
    }
    inline const std::set<std::string> & getVarTable() {
        return varTable;
    }

 protected:
    std::stack<std::string> stack;
    std::vector<std::string> actualRestriction;
    std::set<std::string> varTable;

    std::string opToStr(BinaryOperation::BinaryOperators op);
    std::string boolOpToStr(Conjunction::BoolOperators op);
    std::tuple<std::string,std::string> unaryOpToStr(UnaryOperation::UnaryOperators op);
    std::string equalityOPtoStr(Equality::ComparatorOp op);
    std::string tabulateString(const std::string & str);
    std::string savePrologFile() throw (std::runtime_error);

};

#endif // PROLOGTRANSLATIONSTACK_H
