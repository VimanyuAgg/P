/**
* \file PrtProgram.h
* \brief Defines the representation of P programs in C.
* A P program will be compiled into a set of constant expressions
* using these data structures.
*/
#ifndef PRTPROGRAM_H
#define PRTPROGRAM_H

#ifdef __cplusplus
extern "C"{
#endif

#include "PrtValues.h"

struct PRT_MACHINEINST; /* forward declaration */

/** A PRT_SM_FUN function is a pointer to a P function.
*   context is the current machine context.
*   Returns a non-null pointer if function has a return type. Otherwise returns C null value. Caller frees return.
*/
typedef PRT_VALUE *(PRT_CALL_CONV * PRT_SM_FUN)(_Inout_ struct PRT_MACHINEINST *context);

/** Represents a P event declaration */
typedef struct PRT_EVENTDECL
{
	PRT_UINT32 declIndex;         /**< The index of event in program                                           */
	PRT_STRING name;              /**< The name of this event set                                              */
	PRT_UINT32 eventMaxInstances; /**< The value of maximum instances of the event that can occur in the queue */
	PRT_TYPE   *type;	          /**< The type of the payload associated with this event                      */

	PRT_UINT32 nAnnotations;      /**< Number of annotations                                                   */
	void       **annotations;     /**< An array of annotations                                                 */
} PRT_EVENTDECL;

/** Represents a set of P events and the set packed into a bit vector */
typedef struct PRT_EVENTSETDECL
{
	PRT_UINT32 *packedEvents;  /**< The events packed into an array of ints */
} PRT_EVENTSETDECL;

/** Represents a P variable declaration */
typedef struct PRT_VARDECL
{
	PRT_STRING name;           /**< The name of this variable              */
	PRT_TYPE   *type;          /**< The type of this variable              */

	PRT_UINT32 nAnnotations;   /**< Number of annotations                  */
	void       **annotations;  /**< An array of annotations                */
} PRT_VARDECL;

typedef struct PRT_CASEDECL
{
	PRT_EVENTDECL *triggerEvent;
	struct PRT_FUNDECL *fun;
} PRT_CASEDECL;

typedef struct PRT_RECEIVEDECL
{
	PRT_UINT16 receiveIndex;
	PRT_EVENTSETDECL *caseSet;
	PRT_UINT32 nCases;
	PRT_CASEDECL *cases;
} PRT_RECEIVEDECL;

/** Represents a P function declaration */
typedef struct PRT_FUNDECL
{
	PRT_STRING name;             /**< name (NULL is anonymous)                                              */
	PRT_SM_FUN implementation;   /**< implementation                                                        */
	PRT_UINT32 numParameters;    /**< number of parameters (1 for anonymous functions)                      */
	PRT_UINT32 maxNumLocals;     /**< number of local variables including nested scopes                     */
	PRT_UINT32 numEnvVars;       /**< number of local variables in enclosing scopes (0 for named functions) */
	PRT_TYPE *payloadType;       /**< payload type for anonymous functions (NULL for named functions)       */
	PRT_TYPE *localsNmdTupType;  /**< type of local variables tuple (not including nested scopes)           */
	PRT_UINT32 nReceives;        /**< number of receive statements in body                                  */
	PRT_RECEIVEDECL *receives;   /**< array of receive decls in body                                        */

	PRT_UINT32 nAnnotations;     /**< number of annotations                                                 */
	void       **annotations;    /**< array of annotations                                                  */
} PRT_FUNDECL;

/** Represents a P transition declaration */
typedef struct PRT_TRANSDECL
{
	PRT_UINT32    ownerStateIndex;   /**< The index of owner state in owner machine       */
	PRT_EVENTDECL *triggerEvent;     /**< The trigger event       */
	PRT_UINT32    destStateIndex;    /**< The index of destination state in owner machine */
	PRT_FUNDECL   *transFun;         /**< The function to execute when this transition is triggered */

	PRT_UINT32    nAnnotations;      /**< Number of annotations                         */
	void          **annotations;     /**< An array of annotations                       */
} PRT_TRANSDECL;

/** Represents a P do declaration */
typedef struct PRT_DODECL
{
	PRT_UINT32      ownerStateIndex;   /**< The index of owner state in owner machine              */
	PRT_EVENTDECL   *triggerEvent;     /**< The trigger event             */
	PRT_FUNDECL     *doFun;            /**< The function to execute when this do is triggered  */

	PRT_UINT32      nAnnotations;      /**< Number of annotations                         */
	void            **annotations;     /**< An array of annotations                       */
} PRT_DODECL;

/** Represents a P state declaration */
typedef struct PRT_STATEDECL
{
	PRT_STRING         name;           /**< The name of this state                 */
	PRT_UINT32         nTransitions;   /**< The number of transitions              */
	PRT_UINT32         nDos;           /**< The number of do handlers              */
	PRT_EVENTSETDECL   *defersSet;     /**< The defers set              */
	PRT_EVENTSETDECL   *transSet;      /**< The transition trigger set */
	PRT_EVENTSETDECL   *doSet;         /**< The do trigger set                 */
	PRT_TRANSDECL      *transitions;   /**< The array of transitions                           */
	PRT_DODECL         *dos;           /**< The array of installed actions                     */
	PRT_FUNDECL        *entryFun;      /**< The entry function in owner machine       */
	PRT_FUNDECL        *exitFun;       /**< The exit function in owner machine        */

	PRT_UINT32         nAnnotations;   /**< Number of annotations                              */
	void               **annotations;  /**< An array of annotations                            */
} PRT_STATEDECL;

/** Represents a P machine declaration */
typedef struct PRT_MACHINEDECL
{
	PRT_UINT32       declIndex;         /**< The index of machine in program     */
	PRT_STRING       name;              /**< The name of this machine            */
	PRT_UINT32       nVars;             /**< The number of state variables       */
	PRT_UINT32       nStates;           /**< The number of states                */
	PRT_UINT32       nFuns;             /**< The number of functions             */

	PRT_UINT32       maxQueueSize;      /**< The max queue size                  */
	PRT_UINT32       initStateIndex;    /**< The index of initial state      */
	PRT_VARDECL      *vars;             /**< The array of variable declarations  */
	PRT_STATEDECL    *states;           /**< The array of state declarations     */
	PRT_FUNDECL      **funs;            /**< The array of fun declarations       */

	PRT_UINT32       nAnnotations;      /**< Number of annotations                              */
	void             **annotations;     /**< An array of annotations                            */
} PRT_MACHINEDECL;

/** Represents a P program declaration */
typedef struct PRT_PROGRAMDECL
{
	PRT_UINT32          nEvents;        /**< The number of events      */
	PRT_UINT32          nMachines;      /**< The number of machines    */
	PRT_UINT32          nForeignTypes;  /**< The number of foreign types */
	PRT_EVENTDECL       **events;       /**< The array of events                 */
	PRT_MACHINEDECL     **machines;     /**< The array of machines               */
	PRT_FOREIGNTYPEDECL **foreignTypes; /**< The array of foreign types */
	PRT_UINT32			**linkMap;		/**< stores the link map from symbolicName -> symbolicName -> symbolicName */
	PRT_UINT32			*machineDefMap;		/**< stores the machine definition map from symbolicName -> concrete name */

	PRT_UINT32          nAnnotations;   /**< Number of annotations               */
	void                **annotations;  /**< An array of annotations             */
} PRT_PROGRAMDECL;

#ifdef __cplusplus
}
#endif
#endif

