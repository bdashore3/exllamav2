#ifndef _ext_tp_h
#define _ext_tp_h

#define BROADCAST_KV 0
#define BROADCAST_ID 1
#define BROADCAST_VC 2
#define BROADCAST_RS 3
#define BROADCAST_Q 4

class ExtTPContext
{
public:
    std::vector<std::tuple<int, int, int>> kv_split;
    std::vector<std::tuple<int, int, int>> id_split;
    std::vector<std::tuple<int, int, int>> vc_split;
    std::vector<std::tuple<int, int, int>> rs_split;
    std::vector<std::tuple<int, int, int>> q_split;
    void* pinned_temp;
    size_t pinned_size;
    std::vector<void*> device_temp;
    std::vector<cudaStream_t> streams;

    cudaEvent_t sync_event;
    std::vector<cudaEvent_t> sync_events;
    std::vector<cudaEvent_t> sync_events2;
    std::vector<cudaEvent_t> sync_events3;

    ExtTPContext(
        std::vector<std::tuple<int, int, int>> _kv_split,
        std::vector<std::tuple<int, int, int>> _id_split,
        std::vector<std::tuple<int, int, int>> _vc_split,
        std::vector<std::tuple<int, int, int>> _rs_split,
        std::vector<std::tuple<int, int, int>> _q_split,
        torch::Tensor _pinned_temp,
        std::vector<torch::Tensor> _device_temp,
        std::vector<cudaStream_t> _streams
    );
    ~ExtTPContext();

    void create_events();
};

uintptr_t make_tp_context
(
    std::vector<std::tuple<int, int, int>> kv_split,
    std::vector<std::tuple<int, int, int>> id_split,
    std::vector<std::tuple<int, int, int>> vc_split,
    std::vector<std::tuple<int, int, int>> rs_split,
    std::vector<std::tuple<int, int, int>> q_split,
    torch::Tensor pinned_temp,
    std::vector<torch::Tensor> device_temp,
    std::vector<uintptr_t> streams
);

void free_tp_context(uintptr_t ctx);

void tp_broadcast
(
    uintptr_t tp_context,
    torch::Tensor source,
    int broadcast_type,
    const py::list &targets,
    int dim
);

void tp_gather
(
    uintptr_t tp_context,
    const py::list &inputs,
    int broadcast_type,
    const py::list &targets,
    int broadcast_type_target,
    int dim
);

#endif