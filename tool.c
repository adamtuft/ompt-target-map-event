#include <stdio.h>
#include <omp-tools.h>

/* Entry & exit functions passed back to the OMP runtime */
static int ompt_initialise(ompt_function_lookup_t, int, ompt_data_t *);
static void ompt_finalise(ompt_data_t *);

static void
on_ompt_callback_target_map(
    ompt_id_t                target_id,
    unsigned int             nitems,
    void *                  *host_addr,
    void *                  *device_addr,
    size_t                  *bytes,
    unsigned int            *mapping_flags,
    const void              *codeptr_ra
);

// Tool entrypoint called by OMP runtime during initialisation
ompt_start_tool_result_t *
ompt_start_tool(
    unsigned int omp_version,
    const char  *runtime_version)
{
    fprintf(stderr, "OMPT: Runtime: %s\n", runtime_version);

    static ompt_start_tool_result_t result = {
        .initialize      = &ompt_initialise,
        .finalize        = &ompt_finalise,
        .tool_data.ptr   = NULL
    };

    return &result;
}

// Register callbacks with runtime
static int
ompt_initialise(
    ompt_function_lookup_t lookup, 
    int                    initial_device_num, 
    ompt_data_t *          tool_data)
{   
    ompt_set_callback_t ompt_set_callback = (ompt_set_callback_t) lookup("ompt_set_callback");

    // ompt_set_result_t result = ompt_set_callback(ompt_callback_target_map, (ompt_callback_t) on_ompt_callback_target_map);
    ompt_set_result_t result = ompt_set_never;

#define SET_CALLBACK(event)                                                   \
    do {                                                                      \
        result = ompt_set_callback(event, (ompt_callback_t) on_##event);      \
        fprintf(stderr, "OMPT: set callback %s = %d\n", "on_" #event, result);\
    } while(0);

    SET_CALLBACK(ompt_callback_target_map);

    fprintf(stderr, "OMPT: Set callback for ompt_callback_target_map event with result: %d\n", result);

    return 1;
}

static void
ompt_finalise(ompt_data_t *tool_data)
{
    fprintf(stderr, "OMPT: Finalising tool\n");
    return;
}

/*
The target-map event occurs when a thread maps data to or from a target device.

A thread dispatches a registered ompt_callback_target_map callback for each
occurrence of a target-map event in that thread. The callback occurs in the
context of the target task and has type signature ompt_callback_target_map_t.
*/
static void
on_ompt_callback_target_map(
    ompt_id_t                target_id,
    unsigned int             nitems,
    void *                  *host_addr,
    void *                  *device_addr,
    size_t                  *bytes,
    unsigned int            *mapping_flags,
    const void              *codeptr_ra)
{
    fprintf(stderr, "OMPT: %s\n", __func__);
    return;
}
