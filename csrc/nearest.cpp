#ifdef WITH_PYTHON
#include <Python.h>
#endif
#include <torch/script.h>

#include "extensions.h"

#include "cpu/nearest_cpu.h"

#ifdef WITH_CUDA
#include "cuda/nearest_cuda.h"
#endif

#ifdef _WIN32
#ifdef WITH_PYTHON
#ifdef WITH_CUDA
PyMODINIT_FUNC PyInit__nearest_cuda(void) { return NULL; }
#else
PyMODINIT_FUNC PyInit__nearest_cpu(void) { return NULL; }
#endif
#endif
#endif

CLUSTER_API torch::Tensor nearest(torch::Tensor x, torch::Tensor y, torch::Tensor batch_ptr_x,
                      torch::Tensor batch_ptr_y) {
  if (x.device().is_cuda()) {
#ifdef WITH_CUDA
    return nearest_cuda(x, y, batch_ptr_x, batch_ptr_y);
#else
    AT_ERROR("Not compiled with CUDA support");
#endif
  } else {
    return nearest_cpu(x, y, batch_ptr_x, batch_ptr_y);
  }
}

static auto registry =
    torch::RegisterOperators().op("torch_cluster::nearest", &nearest);
