#include "mediapipe/framework/calculator_framework.h"
#include "mediapipe/framework/port/status.h"
#include "tensorflow/core/framework/tensor.h"
#include "tensorflow/core/framework/types.h"
#include "mediapipe/framework/formats/landmark.pb.h"
#include "mediapipe/framework/formats/detection.pb.h"
#include "mediapipe/framework/formats/matrix.h"

#include <vector>

namespace mediapipe{

    namespace{
        constexpr char NormalizedLandmarks[] = "LANDMARKS";
        constexpr char LandmarkMatrix[] = "MATRIX";
    }
    namespace tf = ::tensorflow;

    class LandmarksToTensorCalculator : public CalculatorBase {
        public:
        LandmarksToTensorCalculator(){};
        ~LandmarksToTensorCalculator(){};

        static ::mediapipe::Status GetContract(CalculatorContract* cc){
            cc->Inputs().Tag(NormalizedLandmarks).Set<std::vector<NormalizedLandmarkList>>();
            // cc->Outputs().Tag(DOUBLE).Set<double>();
            cc->Outputs().Tag(LandmarkMatrix).Set<Matrix>();
            return ::mediapipe::OkStatus();
        }
        ::mediapipe::Status Open(CalculatorContext* cc){

            return ::mediapipe::OkStatus();
        }
        ::mediapipe::Status Process(CalculatorContext* cc){
            std::vector<NormalizedLandmarkList> hands = cc->Inputs().Tag(NormalizedLandmarks).Get<std::vector<NormalizedLandmarkList>>();
            Matrix bob;
            bob.resize(1, 42);
            if (hands.size() == 1){
                auto hand = hands.at(0);
                for(int i = 0; i < 21; i++){              
                    bob(0, i*2) = hand.landmark(i).x();
                    bob(0, i*2+1) = hand.landmark(i).y();
                }
            }else if(hands.size() == 0){
                for(int i = 0; i < 21; i++){              
                    bob(0, i*2) = 0;
                    bob(0, i*2+1) = 0;
                }
            }else{
                for(int i = 0; i < 21; i++){              
                    bob(0, i*2) = -1;
                    bob(0, i*2+1) = -1;
                }
            }
            

            std::unique_ptr<Matrix> output_stream_collection = std::make_unique<Matrix>(bob); 
            cc -> Outputs().Tag(LandmarkMatrix).Add(output_stream_collection.release(), cc->InputTimestamp());

            // LOG(INFO) << "MATRIX" << bob.rows() << ", " << bob.cols() << bob(0, 0);
            // LOG(INFO) << bob(0, 0) << ", " << bob(1, 0) << ", "<< bob(0, 1) << ", "<< bob(1, 1) << ", " << bob(2, 1) << ", " << bob(1, 2) << bob(100, 100) << "\n";
            LOG(INFO) << bob << "\n";
            
            // tf::TensorShape tensor_shape = tf::TensorShape({1, 42});
            

            // auto output_tensors_cpu = ::absl::make_unique<std::vector<tf::Tensor>>();
            // auto output = output_tensors_cpu->at(0);
            // tf::Tensor tensore;
            // if(hands.size() == 0){
            //     return ::mediapipe::OkStatus();
            // }
            // auto hand = hands.at(0);
            // for(unsigned int i = 0; i < hand.landmark_size(); i++){
            //     output->tensor<float, 2>()(i) = hand.landmark(i).x();
            //     output->tensor<float, 2>()(i*2+1) = hand.landmark(i).y();
            // }
            // cc->Outputs().Tag(LandmarkMatrix).Add(output.release(), cc->InputTimestamp());
            
            return ::mediapipe::OkStatus();
        }
        ::mediapipe::Status Close(CalculatorContext* cc){
            return ::mediapipe::OkStatus();
        }

        private:

    };
    
    REGISTER_CALCULATOR(LandmarksToTensorCalculator);
}