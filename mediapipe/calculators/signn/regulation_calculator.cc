#include "mediapipe/framework/calculator_framework.h"
#include "mediapipe/framework/formats/landmark.pb.h"


#include <vector>
#include <fstream>




namespace mediapipe{

    namespace{
        constexpr char LandmarksHistory[] = "LANDMARKS_HISTORY";
        constexpr char DoubleLandmarksHistory[] = "DOUBLE_LANDMARKS_HISTORY";
    }


    std::vector<double> sublistRegulation(std::vector<double> input, int output_frame_count){
    std::vector<double> slopes;
    std::vector<double> offsets;
    int input_len = input.size();
    int i = 0;
    for(int j = 0; j < input_len; j++){
        offsets.push_back(input[i]);

        if(i == input_len - 1)
        slopes.push_back(0);
        else
        slopes.push_back(input[i+1] - input[i]);

        i += 1;
    }

    std::vector<double> iterpolated_list;
    double step_count = double(input_len - 1)/double(output_frame_count - 1);
    double pointer = 0;
    while(pointer <= input_len - 1){
        int current_index = int(pointer)/1;
        double current_value = slopes[current_index] * double(pointer - current_index) + offsets[current_index];
        iterpolated_list.push_back(current_value);
        pointer += step_count;
    }

    return iterpolated_list;

    }

    std::vector<std::vector<double>> regulation(std::vector<std::vector<double>> input_frames, int output_frame_count, int coordinates_per_frame){
    std::vector<std::vector<double>> master_list;
    std::vector<std::vector<double>> slices;
    if(input_frames.size() <= 1){
        double new_size_1 = 0;
        double new_size_2 = 0;
        double new_size_3 = 0;
        if(input_frames.size() == 1){
            new_size_1 = input_frames[0][0];
            new_size_2 = input_frames[0][1];
            new_size_3 = input_frames[0][2];
        }
        for(int i = 0; i < output_frame_count; i++){
            std::vector<double> frame;
            for(int j = 0; j < coordinates_per_frame/3; j++){
                frame.push_back(new_size_1);
                frame.push_back(new_size_2);
                frame.push_back(new_size_3);
            }
            master_list.push_back(frame);
        }
        return master_list;
    }

    for(int s = 0; s < coordinates_per_frame; s++){
        std::vector<double> sliced;
        for(int i = 0; i < input_frames.size(); i++)
        sliced.push_back(input_frames[i][s]);
        slices.push_back(sublistRegulation(sliced,output_frame_count));
    }
    for(int j = 0; j < output_frame_count; j++){
        std::vector<double> newFrames;
        for(int t = 0; t < coordinates_per_frame; t++){
        newFrames.push_back(slices[t][j]);
        }
        master_list.push_back(newFrames);
    }
    return master_list;
    }

    class RegulationCalculator : public CalculatorBase {
        public:
        RegulationCalculator(){};
        ~RegulationCalculator(){};

        static ::mediapipe::Status GetContract(CalculatorContract* cc){
            cc->Inputs().Tag(LandmarksHistory).Set<std::vector<NormalizedLandmarkList>>();
            cc->Outputs().Tag(DoubleLandmarksHistory).Set<std::vector<std::vector<double>>>();
            return ::mediapipe::OkStatus();
        }
        ::mediapipe::Status Open(CalculatorContext* cc){
            // const auto& options = cc->Options<::mediapipe::LandmarkHistoryCalculatorOptions>();
            // hand_history = TimedQueue<NormalizedLandmarkList>(options.memory_in_seconds());
            
            return ::mediapipe::OkStatus();
        }
        ::mediapipe::Status Process(CalculatorContext* cc){        
            std::vector<NormalizedLandmarkList> hands = cc->Inputs().Tag(LandmarksHistory).Get<std::vector<NormalizedLandmarkList>>();
            std::vector<std::vector<double>> unregulatedHands;

            for(int i = 0; i < hands.size(); i++){
                auto hand_obj = hands.at(i);
                std::vector<double> hand;
                for(int j = 0; j < hand_obj.landmark_size(); j++){
                    hand.push_back(hand_obj.landmark(j).x());
                    hand.push_back(hand_obj.landmark(j).y());
                    // hand.push_back(hand_obj.landmark(j).z());
                }
                unregulatedHands.push_back(hand);
            }

            auto regulatedHands = regulation(unregulatedHands, 60, 42);           
            std::unique_ptr<std::vector<std::vector<double>>> output_stream_collection = std::make_unique<std::vector<std::vector<double>>>(regulatedHands); 
            cc -> Outputs().Tag(DoubleLandmarksHistory).Add(output_stream_collection.release(), cc->InputTimestamp());
            return ::mediapipe::OkStatus();
        }
        ::mediapipe::Status Close(CalculatorContext* cc){
            
            return ::mediapipe::OkStatus();
        }
        private:
        std::ofstream file;



    };
    REGISTER_CALCULATOR(RegulationCalculator);
}