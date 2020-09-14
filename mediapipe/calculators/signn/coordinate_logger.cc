#include "mediapipe/framework/calculator_framework.h"
#include "mediapipe/framework/formats/landmark.pb.h"
#include "mediapipe/framework/formats/detection.pb.h"
#include "mediapipe/framework/port/ret_check.h"
#include "mediapipe/framework/deps/file_path.h"
#include "mediapipe/framework/port/file_helpers.h"
#include "mediapipe/calculators/signn/coordinate_logger_calculator.pb.h"


#include <string>
#include <iostream>
#include <fstream>

#include <chrono>
#include <ctime>   

namespace mediapipe{

    namespace{
        constexpr char NormalizedLandmarks[] = "LANDMARKS";
    }


    class CoordinateLoggerCalculator : public CalculatorBase {
        public:
        CoordinateLoggerCalculator(){};
        ~CoordinateLoggerCalculator(){};

        static ::mediapipe::Status GetContract(CalculatorContract* cc){
            cc->Inputs().Tag(NormalizedLandmarks).Set<NormalizedLandmarkList>();
            return ::mediapipe::OkStatus();
        }
        ::mediapipe::Status Open(CalculatorContext* cc){
            const auto& options = cc->Options<::mediapipe::CoordinateLoggerCalculatorOptions>();
            bool absolute_path = options.use_absolute_path();
            std::string logger_path = options.logger_path();
            std::string fileName = options.filename();
            std::string log_path;
            if(! absolute_path){
                log_path = mediapipe::file::JoinPath("./",
                                                            logger_path);
            }else{
                log_path = logger_path;
            }
            if(fileName == ""){
                auto now = std::chrono::system_clock::now();
                time_t now_as_time = std::chrono::system_clock::to_time_t(now);
                std::string time_as_string = (std::string) ctime(&now_as_time);
                // time_as_string = Fri Nov 29 01:33:30 2019
                bool first_space_hit = false;
                for(char c: time_as_string){
                    if(first_space_hit){
                        if(c == ' '){
                            fileName += '_';
                        }else{
                            fileName += c;
                        }
                    }
                    if(c == ' '){
                        first_space_hit = true;
                    }
                }
                // fileName = Nov_29_01:33:30_2019
                fileName += ".json";
                // fileName = Nov_29_01:33:30_2019.txt
            }

            log_path = mediapipe::file::JoinPath(log_path, fileName);
            LOG(INFO) << "Opening logger at the following path: " << log_path;
            file.open(log_path);
            file << "[";


            return ::mediapipe::OkStatus();
        }
        ::mediapipe::Status Process(CalculatorContext* cc){
            NormalizedLandmarkList hand = cc->Inputs().Tag(NormalizedLandmarks).Get<NormalizedLandmarkList>();
            if(firstwrite){
                file << "[";
                firstwrite = false;
            }else{
                file << ", [";
            }

            for(int j = 0; j < hand.landmark_size(); j++){
                file << std::fixed << hand.landmark(j).x() << ", " << std::fixed << hand.landmark(j).y() << ", " << std::fixed << hand.landmark(j).z();
                if(j + 1 < hand.landmark_size()){
                    file << ", ";
                }
            }
            file << "]";

            return ::mediapipe::OkStatus();
        }
        ::mediapipe::Status Close(CalculatorContext* cc){
            LOG(INFO) << "Closing logger";
            file << "]";
            file.close();
            return ::mediapipe::OkStatus();
        }

        private:
        bool firstwrite = true;
        std::ofstream file;

    };
    REGISTER_CALCULATOR(CoordinateLoggerCalculator);
}