#include "mediapipe/framework/calculator_framework.h"
#include "mediapipe/util/render_data.pb.h"



namespace mediapipe{

    namespace{
        constexpr char kRenderDataTag[] = "RENDER_DATA";
    }

    class SignnEndGateCalculator: public CalculatorBase {
        public:
        SignnEndGateCalculator(){};
        ~SignnEndGateCalculator(){};

        static ::mediapipe::Status GetContract(CalculatorContract* cc){
            for (int i = 0; i < cc->Inputs().NumEntries(); ++i) {
                cc->Inputs().Index(i).Set<RenderData>();
            }
            cc->Outputs().Tag(kRenderDataTag).Set<RenderData>();
            return ::mediapipe::OkStatus();
        }
        ::mediapipe::Status Open(CalculatorContext* cc){
            return ::mediapipe::OkStatus();
        }
        ::mediapipe::Status Process(CalculatorContext* cc){
            for(int i = 0; i < cc->Inputs().NumEntries(); i++){
                if(! cc->Inputs().Index(i).IsEmpty()){
                    LOG(INFO) << i;
                    auto last_data = cc->Inputs().Index(i).Get<RenderData>();
                    auto output_data = absl::make_unique<RenderData>(last_data);
                    cc->Outputs().Tag(kRenderDataTag).Add(output_data.release(), cc->InputTimestamp());
                    return ::mediapipe::OkStatus();
                }
            }

        }
        ::mediapipe::Status Close(CalculatorContext* cc){
            return ::mediapipe::OkStatus();
        }

        private:


    };
    REGISTER_CALCULATOR(SignnEndGateCalculator);

}