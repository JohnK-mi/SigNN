#include "absl/memory/memory.h"
#include "absl/strings/str_cat.h"
#include "absl/strings/str_join.h"
#include "mediapipe/framework/calculator_framework.h"
#include "mediapipe/framework/calculator_options.pb.h"
#include "mediapipe/framework/formats/detection.pb.h"
#include "mediapipe/framework/formats/location_data.pb.h"
#include "mediapipe/framework/port/ret_check.h"
#include "mediapipe/util/color.pb.h"
#include "mediapipe/util/render_data.pb.h"



namespace mediapipe{

    namespace{
        constexpr char STRING[] = "STRING";
        constexpr char kRenderDataTag[] = "RENDER_DATA";
    }

    class StringToRenderDataCalculator : public CalculatorBase {
        public:
        StringToRenderDataCalculator(){};
        ~StringToRenderDataCalculator(){};

        static ::mediapipe::Status GetContract(CalculatorContract* cc){
            cc->Inputs().Tag(STRING).Set<std::string>();
            cc->Outputs().Tag(kRenderDataTag).Set<RenderData>();
            return ::mediapipe::OkStatus();
        }
        ::mediapipe::Status Open(CalculatorContext* cc){
            return ::mediapipe::OkStatus();
        }
        ::mediapipe::Status Process(CalculatorContext* cc){
            std::string given_string = cc->Inputs().Tag(STRING).Get<std::string>();
            auto render_data = absl::make_unique<RenderData>();
            auto* label_annotation = render_data->add_render_annotations();
            auto* text = label_annotation->mutable_text();
            text->set_display_text(given_string);
            text->set_font_height(35);
            text->set_normalized(false);
            text->set_left(150);
            text->set_baseline(450);
            label_annotation->mutable_color()->set_r(0);
            label_annotation->mutable_color()->set_g(0);
            label_annotation->mutable_color()->set_b(0);
            label_annotation->set_thickness(1);

            auto* label_annotation_other = render_data->add_render_annotations();
            auto* text_other = label_annotation_other->mutable_text();
            text_other->set_display_text(given_string);
            text_other->set_font_height(35);
            text_other->set_normalized(false);
            text_other->set_left(199);
            text_other->set_baseline(451);
            label_annotation->mutable_color()->set_r(256);
            label_annotation->mutable_color()->set_g(256);
            label_annotation->mutable_color()->set_b(256);
            label_annotation->set_thickness(1);

            cc->Outputs().Tag(kRenderDataTag).Add(render_data.release(), cc->InputTimestamp());
            return ::mediapipe::OkStatus();
        }
        ::mediapipe::Status Close(CalculatorContext* cc){
            return ::mediapipe::OkStatus();
        }

        private:
        

    };
    REGISTER_CALCULATOR(StringToRenderDataCalculator);
}