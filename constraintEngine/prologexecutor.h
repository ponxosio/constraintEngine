#ifndef PROLOGEXECUTOR_H
#define PROLOGEXECUTOR_H

#include <memory>
#include <string>
#include <set>
#include <unordered_map>

#include <QTemporaryFile>

#include <SWI-cpp.h>

#include <fluidicmachinemodel/constraintssolverinterface/routingengine.h>

#include "constraintengine/constraintsenginelibrary_global.h"

/**
 * @brief The PrologExecutor class connects with the SWI-Prolog clpfd library that is use as constraint solver.
 *
 * The PrologExecutor class connects with the SWI-Prolog clpfd library(http://www.swi-prolog.org/man/clpfd.html) that is use as
 * constraint solver. This class implements the RoutingEngine interface from FluidicMachineModel library is in charge of resolving
 * a constraints problems: once a set of flows are sets on a machine an state for each: valve, pump, container and tube is calculated
 * so all the flows are mantain.
 *
 * In order to do so, creates a temporary file with a prolog predicate that contains variables representing each: valve, pump, tube and
 * container of a machine and loads this file in the SWI-prolog interprete. Each time a new set flows need to be processed, the predicate
 * of the temporary file is invoqued with the value of the variables that the flows sets, thepredicate returns the value of the rests of
 * components of the system so the flows are mantained using the minimal number of pumps and valves.
 *
 * @sa RoutingEngine.
 */
class PROLOGEXECUTOR_EXPORT PrologExecutor : public RoutingEngine
{
public:
    /**
     * @brief createEngine this methdod only needs to be invoqued once in a program execution, starts the swi-prolog interpreter.
     * @param appName path of the executable being call, arg[0] of the main method.
     */
    static void createEngine(const std::string & appName);
    /**
     * @brief destoryEngine this methdod only needs to be invoqued once in a program execution, close the swi-prolog interpreter.
     */
    static void destoryEngine();

    /**
     * @brief PrologExecutor creates a new interface to communicate with SWI-Prolog clpfd library.
     *
     * PrologExecutor loads a new file with a predicate into the swi-prolog interpreter and reads all
     * the variables the predicate has, these variables are always order in alphabetical order, because
     * the interprete use the position of a variable instead of the name of it to differentiate them.
     *
     * For example a machine that has 2 container and a pump will have a predicate like this:
     * "machine(C_1,C_2,P_3):-" when communicating with swi-prolog interpreter if we want to set C_2 to
     * certain value we need to call the predicate with the second variable as the desired value.
     *
     * @param temporaryFile unique pointer that contains the temporary file with the predicate that calculate
     * constraints for a given machine, first the file is loaded into the interpreter, them the pointer is held
     * by this class so we can asure that the temporary file exist all the time this object exists.
     * @param varTable name of the variables used in the predicate, in alphabetical order the same order the predicate has them.
     *
     * @sa QTemporaryFile, @sa RoutingEngine.
     */
    PrologExecutor(std::unique_ptr<QTemporaryFile> temporaryFile, const std::set<std::string> & varTable);
    /**
     * @brief ~PrologExecutor does not call destroyEngine(), deletes the temporary file.
     *
     * ~PrologExecutor does not call destroyEngine(), when this methdos is called the unique pointer that
     * contains the temporary file is destroy, this tells QTemporaryFile to delete the physical file with
     * the predicate.
     */
    virtual ~PrologExecutor();

    /**
     * @brief calculateNewRoute executes the predicate in the intrepeter and returns the new state of the machine.
     *
     * calculateNewRoute makes a new call to the predicate in the temprary file, with the values for the variables recived as input
     * as grounded variables and the rest as free ones, if possible returns the state of the machine that calculated by the swi-prolog
     * interpreter. If the interprete throw any exception this is throw as a runtime_error to C++ to be handled.
     *
     * @param inputStates map with the name as key and the value of the variables that are going to be ground when calling the swi-prolog
     * interpreter.
     * @param outStates map with the name as key and the value of every variable at the predicate fill with the corresponding value calculated
     * by the swi-prolog interpreter. If no solution is found for the gicen input, the map is returned intact.
     * @return true if a state compatible with the input data is found, false otherwise.
     */
    virtual bool calculateNewRoute(const std::unordered_map<std::string, long long> & inputStates,
                                   std::unordered_map<std::string, long long> & outStates) throw(std::runtime_error);

private:
    /**
     * @brief engine objects that contains the swi-prolog interpreter
     */
    static PlEngine* engine;

    /**
     * @brief fileName path to the temprary file containing the predicate that makes the calculus in swi-prolog.
     */
    std::string fileName;
    /**
     * @brief varPositionTable map with the name of a variable as key and the position in the prolog predicate as value
     */
    std::unordered_map<std::string, int> varPositionTable;
    /**
     * @brief file pointer to the temporary file, this is kept so the tempory file is not deleted until this
     * object is destroyed
     */
    std::unique_ptr<QTemporaryFile> file;
};

#endif // PROLOGEXECUTOR_H
