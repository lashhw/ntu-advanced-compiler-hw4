#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <random>
#include <regex>
#include <cassert>

void copy_prop(float, float, float*);
void cse(float, float, float, float, float*, float*);
void dce(float, float, float, float, float*);
void lagrange(float, float, float, float, float, float, float, float*);
void matrix(float, float, float, float, float, float, float, float, float*, float*, float*, float*);
void ray_trig(float, float, float, float, float, float, float, float, float, float, float, float, float, float, float, float, float*, float*, float*);
void rotation(float, float, float, float, float, float, float, float, float*, float*, float*);
void sobel(float, float, float, float, float, float, float, float, float, float, float*);

struct Node {
    std::string type;  // "in", "out", "op"
    std::string op;  // "+", "-", "*", "/"
    std::vector<std::string> inputs;
    float value;
};

struct GraphValidator {
    std::unordered_map<std::string, Node> nodes;
    std::vector<std::string> inputNodes;
    std::vector<std::string> outputNodes;

    GraphValidator(const std::string& filename) {
        std::ifstream file(filename);
        for (std::string line; std::getline(file, line); )
            parseDotLine(line);
    }

    void parseDotLine(const std::string& line) {
        static std::regex nodeRegex(R"~(^\s*(\w+)\s*\[label="([^"]+)")~");
        static std::regex edgeRegex(R"~(^\s*(\w+)\s*->\s*(\w+))~");

        std::smatch matches;
        if (std::regex_search(line, matches, nodeRegex)) {
            std::string nodeName = matches[1];
            std::string label = matches[2];
            
            Node& node = nodes[nodeName];
            if (nodeName.substr(0,2) == "in") {
                node.type = "in";
                inputNodes.push_back(nodeName);
            } else if (nodeName.substr(0,3) == "out") {
                node.type = "out";
                outputNodes.push_back(nodeName);
            } else if (nodeName.substr(0,2) == "op") {
                node.type = "op";
                node.op = label;
            } else {
                assert(false);
            }
        } else if (std::regex_search(line, matches, edgeRegex)) {
            std::string from = matches[1];
            std::string to = matches[2];

            nodes[to].inputs.push_back(from);
        }
    }

    float evaluateNode(const std::string& nodeName) {
        Node& node = nodes[nodeName];
        
        if (node.type == "in") {
            return node.value;
        }

        if (node.type == "out") {
            return evaluateNode(node.inputs[0]);
        }

        if (node.type == "op") {
            float op1 = evaluateNode(node.inputs[0]);
            float op2 = evaluateNode(node.inputs[1]);
            
            if (node.op == "+") return op1 + op2;
            if (node.op == "-") return op1 - op2;
            if (node.op == "*") return op1 * op2;
            if (node.op == "/") return op1 / op2;

            assert(false);
        }

        assert(false);
    }

    bool validate(const std::string& testName, int numTests) {
        std::mt19937 gen(12345);
        std::uniform_real_distribution<float> dis(-1.0, 1.0);

        for (int test = 0; test < numTests; test++) {
            float in[16];
            for (float &x : in)
                x = dis(gen);

            float out[4];
            for (float &x : out)
                x = 0.0f;

            if (testName == "copy_prop") {
                copy_prop(in[0], in[1], &out[0]);
            } else if (testName == "cse") {
                cse(in[0], in[1], in[2], in[3], &out[0], &out[1]);
            } else if (testName == "dce") {
                dce(in[0], in[1], in[2], in[3], &out[0]);
            } else if (testName == "lagrange") {
                lagrange(in[0], in[1], in[2], in[3], in[4], in[5], in[6], &out[0]);
            } else if (testName == "matrix") {
                matrix(in[0], in[1], in[2], in[3], in[4], in[5], in[6], in[7], &out[0], &out[1], &out[2], &out[3]);
            } else if (testName == "ray_trig") {
                ray_trig(in[0], in[1], in[2], in[3], in[4], in[5], in[6], in[7], in[8], in[9], in[10], in[11], in[12], in[13], in[14], in[15], &out[0], &out[1], &out[2]);
            } else if (testName == "rotation") {
                rotation(in[0], in[1], in[2], in[3], in[4], in[5], in[6], in[7], &out[0], &out[1], &out[2]);
            } else if (testName == "sobel") {
                sobel(in[0], in[1], in[2], in[3], in[4], in[5], in[6], in[7], in[8], in[9], &out[0]);
            } else {
                std::cout << "Unknown test name: " << testName << std::endl;
                return false;
            }

            std::unordered_map<std::string, float> inputMapping = {
                {"in0", in[0]}, {"in1", in[1]}, {"in2", in[2]}, {"in3", in[3]},
                {"in4", in[4]}, {"in5", in[5]}, {"in6", in[6]}, {"in7", in[7]},
                {"in8", in[8]}, {"in9", in[9]}, {"in10", in[10]}, {"in11", in[11]},
                {"in12", in[12]}, {"in13", in[13]}, {"in14", in[14]}, {"in15", in[15]}
            };

            for (const auto& nodeName : inputNodes)
                nodes[nodeName].value = inputMapping[nodeName];

            for (const auto& outNode : outputNodes)
                nodes[outNode].value = evaluateNode(outNode);

            std::unordered_map<std::string, float*> outputMapping = {
                {"out0", &out[0]}, {"out1", &out[1]}, {"out2", &out[2]}, {"out3", &out[3]}
            };

            for (const auto& [outNode, _] : outputMapping) {
                float expected = *outputMapping[outNode];
                float actual = nodes.find(outNode) == nodes.end() ? 0.0f : nodes[outNode].value;
                
                // std::cout << "expected: " << expected << ", actual: " << actual << std::endl;
                if (expected != actual) {
                    std::cout << "Mismatch found for " << outNode << ": expected " << expected << ", got " << actual << std::endl;
                    return false;
                }
            }
        }
        
        std::cout << "All " << numTests << " tests passed!" << std::endl;
        return true;
    }
};

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cout << "Usage: " << argv[0] << " test_name dot_file num_tests" << std::endl;
        return 1;
    }
    std::string testName = argv[1];
    std::string dotFile = argv[2];
    int numTests = std::stoi(argv[3]);

    GraphValidator validator(dotFile);
    return validator.validate(testName, numTests) ? 0 : 1;
}