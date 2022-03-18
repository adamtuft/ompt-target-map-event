#include <stdio.h>
#include <omp-tools.h>
#include "include/ompt-callbacks.h"
#include "include/macros.h"

static int ompt_initialise(ompt_function_lookup_t, int, ompt_data_t *);
static void ompt_finalise(ompt_data_t *);
static uint64_t get_unique_thread_id(void);

// Invoked by OpenMP runtime
ompt_start_tool_result_t *
ompt_start_tool(
    unsigned int omp_version,
    const char  *runtime_version)
{
    fprintf(stderr, "OMPT: Runtime: %s\n", runtime_version);

    static ompt_start_tool_result_t result;
    result.initialize    = &ompt_initialise;
    result.finalize      = &ompt_finalise;
    result.tool_data.ptr = NULL;

    return &result;
}

// Initialise tool, obtain runtime entrypoints and register callbacks
static int
ompt_initialise(
    ompt_function_lookup_t lookup, 
    int                    initial_device_num, 
    ompt_data_t *          tool_data)
{   
    ompt_set_callback_t ompt_set_callback = (ompt_set_callback_t) lookup("ompt_set_callback");

    ompt_set_result_t result = ompt_set_never;

    // Register callbacks and print registration result
    // SET_CALLBACK(ompt_callback_thread_begin);
    // SET_CALLBACK(ompt_callback_thread_end);
    // SET_CALLBACK(ompt_callback_parallel_begin);
    // SET_CALLBACK(ompt_callback_parallel_end);
    // SET_CALLBACK(ompt_callback_target);
    // SET_CALLBACK(ompt_callback_target_submit);
    // SET_CALLBACK(ompt_callback_target_data_op);
    // SET_CALLBACK(ompt_callback_target_map);
    SET_CALLBACK(ompt_callback_device_initialize);
    SET_CALLBACK(ompt_callback_device_finalize);
    SET_CALLBACK(ompt_callback_device_load);
    SET_CALLBACK(ompt_callback_device_unload);

    return 1;
}

static void
ompt_finalise(
    ompt_data_t *tool_data)
{
    LOG_MESSAGE("Tool finalised");
    return;
}

static void
on_ompt_callback_buffer_request( 
    int device_num, 
    ompt_buffer_t **buffer, 
    size_t *bytes)
{
    *bytes = 0; // do not provide an event buffer
    LOG_MESSAGE();
    return;
}

static void
on_ompt_callback_buffer_complete( 
    int device_num, 
    ompt_buffer_t *buffer, 
    size_t bytes, 
    ompt_buffer_cursor_t begin, 
    int buffer_owned)
{
    LOG_MESSAGE();
    return;
}

static void
on_ompt_callback_device_initialize(
    int                      device_num,
    const char              *type,
    ompt_device_t           *device,
    ompt_function_lookup_t   lookup,
    const char              *documentation)
{
    LOG_MESSAGE("initialising device %d: %s", device_num, type);
    fprintf(stderr, "Device: %s\n"
                    "Documentation:%s\n", type, documentation);

    if (lookup == NULL)
    {
        LOG_MESSAGE("[dev %d] tracing not supported", device_num);
        return;
    }

    LOG_MESSAGE("[dev %d] looking up device tracing entrypoints", device_num);

    ompt_get_device_num_procs_t get_device_num_procs = (ompt_get_device_num_procs_t) lookup("ompt_get_device_num_procs_t");
    ompt_get_device_time_t get_device_time = (ompt_get_device_time_t) lookup("ompt_get_device_time_t");
    ompt_translate_time_t translate_time = (ompt_translate_time_t) lookup("ompt_translate_time_t");
    ompt_set_trace_ompt_t set_trace_ompt = (ompt_set_trace_ompt_t) lookup("ompt_set_trace_ompt_t");
    ompt_set_trace_native_t set_trace_native = (ompt_set_trace_native_t) lookup("ompt_set_trace_native_t");
    ompt_start_trace_t start_trace = (ompt_start_trace_t) lookup("ompt_start_trace_t");
    ompt_pause_trace_t pause_trace = (ompt_pause_trace_t) lookup("ompt_pause_trace_t");
    ompt_flush_trace_t flush_trace = (ompt_flush_trace_t) lookup("ompt_flush_trace_t");
    ompt_stop_trace_t stop_trace = (ompt_stop_trace_t) lookup("ompt_stop_trace_t");
    ompt_advance_buffer_cursor_t advance_buffer_cursor = (ompt_advance_buffer_cursor_t) lookup("ompt_advance_buffer_cursor_t");
    ompt_get_record_type_t get_record_type = (ompt_get_record_type_t) lookup("ompt_get_record_type_t");
    ompt_get_record_ompt_t get_record_ompt = (ompt_get_record_ompt_t) lookup("ompt_get_record_ompt_t");
    ompt_get_record_native_t get_record_native = (ompt_get_record_native_t) lookup("ompt_get_record_native_t");
    ompt_get_record_abstract_t get_record_abstract = (ompt_get_record_abstract_t) lookup("ompt_get_record_abstract_t");

    LOG_MESSAGE("[dev %d] get_device_num_procs:   %2savailable [%p]", device_num, (get_device_num_procs == NULL ? "un" : ""), get_device_num_procs);
    LOG_MESSAGE("[dev %d] get_device_time:        %2savailable [%p]", device_num, (get_device_time == NULL ? "un" : ""), get_device_time);
    LOG_MESSAGE("[dev %d] translate_time:         %2savailable [%p]", device_num, (translate_time == NULL ? "un" : ""), translate_time);
    LOG_MESSAGE("[dev %d] set_trace_ompt:         %2savailable [%p]", device_num, (set_trace_ompt == NULL ? "un" : ""), set_trace_ompt);
    LOG_MESSAGE("[dev %d] set_trace_native:       %2savailable [%p]", device_num, (set_trace_native == NULL ? "un" : ""), set_trace_native);
    LOG_MESSAGE("[dev %d] start_trace:            %2savailable [%p]", device_num, (start_trace == NULL ? "un" : ""), start_trace);
    LOG_MESSAGE("[dev %d] pause_trace:            %2savailable [%p]", device_num, (pause_trace == NULL ? "un" : ""), pause_trace);
    LOG_MESSAGE("[dev %d] flush_trace:            %2savailable [%p]", device_num, (flush_trace == NULL ? "un" : ""), flush_trace);
    LOG_MESSAGE("[dev %d] stop_trace:             %2savailable [%p]", device_num, (stop_trace == NULL ? "un" : ""), stop_trace);
    LOG_MESSAGE("[dev %d] advance_buffer_cursor:  %2savailable [%p]", device_num, (advance_buffer_cursor == NULL ? "un" : ""), advance_buffer_cursor);
    LOG_MESSAGE("[dev %d] get_record_type:        %2savailable [%p]", device_num, (get_record_type == NULL ? "un" : ""), get_record_type);
    LOG_MESSAGE("[dev %d] get_record_ompt:        %2savailable [%p]", device_num, (get_record_ompt == NULL ? "un" : ""), get_record_ompt);
    LOG_MESSAGE("[dev %d] get_record_native:      %2savailable [%p]", device_num, (get_record_native == NULL ? "un" : ""), get_record_native);
    LOG_MESSAGE("[dev %d] get_record_abstract:    %2savailable [%p]", device_num, (get_record_abstract == NULL ? "un" : ""), get_record_abstract);

    if (set_trace_ompt != NULL)
    {
        ompt_set_result_t result = ompt_set_never;
        const unsigned int enable = 1;
        LOG_MESSAGE("[dev %d] enabling OMPT trace records", device_num);
        for (int event=1; event<=32; event++)
        {
            result = set_trace_ompt(device, enable, event);
            LOG_MESSAGE("[dev %d] [%p] enable event %u = %d",
                device_num, device, event, result);
        }
    }

    return;
}

static void
on_ompt_callback_device_finalize(
    int                      device_num)
{
    LOG_MESSAGE("finalising device %d", device_num);
    return;
}

static void
on_ompt_callback_device_load(
    int                      device_num,
    const char              *filename,
    int64_t                  offset_in_file,
    void                    *vma_in_file,
    size_t                   bytes,
    void                    *host_addr,
    void                    *device_addr,
    uint64_t                 module_id)
{
    LOG_MESSAGE();
    return;
}

static void
on_ompt_callback_device_unload(
    int                      device_num,
    uint64_t                 module_id)
{
    LOG_MESSAGE();
    return;
}
