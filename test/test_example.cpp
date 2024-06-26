#include "infer/MultipleInferenceInstances.h"
#include "trt/yolo/YoloDetectPipeline.h"
#include "trt/yolo/YoloDetectionInfer.h"

int main(int argc, char** argv) {

    std::string input_stream_url(argv[1]);
    std::string output_stream_url(argv[2]);
    std::string model_path(argv[3]);
    std::string label_path(argv[4]);

    int         max_batch_size    = 10;    // 最大batch数
    float       config_threshold  = 0.25;  // 检测阈值
    float       nms_threshold     = 0.5;   // nms阈值

    // 模型实例数量列表，列表为模型实例数，每个元素代表该模型实例在哪张显卡上的下标
    std::vector<int> device_list{0, 0, 1, 1};
    auto             type = infer::YoloType::V5;  // 模型类型

    // 创建多卡多实例推理对象
    auto trt_instance =
        std::make_shared<infer::MultipleInferenceInstances<infer::YoloDetectionInfer>>(
            "trt_instance", device_list, model_path, label_path, type, config_threshold,
            nms_threshold, max_batch_size);
    if (!trt_instance->init()) {
        std::cout << "init failed" << std::endl;
        return -1;
    }

    // 创建处理pipeline
    auto pipeline = std::make_shared<pipeline::YoloDetectPipeline>(
        "test_pipeline", input_stream_url, output_stream_url, trt_instance);

    // 启动流水线
    pipeline->Start();

    getchar();
}