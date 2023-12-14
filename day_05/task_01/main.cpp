
#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <numeric>
#include <regex>
#include <sstream>
#include <vector>
using namespace std;

struct Mapping {
  unsigned long long source;
  unsigned long long destination;
  unsigned long long range;
};
void ParseInput(string filename, vector<unsigned long long> &seeds,
                vector<Mapping> &seed_to_soil,
                vector<Mapping> &soil_to_fertilizer,
                vector<Mapping> &fertilizer_to_water,
                vector<Mapping> &water_to_light,
                vector<Mapping> &light_to_temperature,
                vector<Mapping> &temperature_to_humidity,
                vector<Mapping> &humidity_to_location) {

  auto ifs = ifstream(filename);
  if (!ifs.is_open()) {
    cout << "Error opening file.\n";
    exit(-1);
  }

  std::stringstream buffer;
  buffer << ifs.rdbuf();
  string input_str = buffer.str();
  auto pos = size_t{0};

  auto add_mapping = [](std::string text, vector<Mapping> &m) {
    auto nums = std::regex("([0-9]+)");
    std::vector<unsigned long long> vs{};
    for (std::smatch sm; std::regex_search(text, sm, nums);) {
      vs.push_back(stoull(sm.str().c_str()));
      text = sm.suffix();
    }

    for (auto i = size_t{0}; i < vs.size(); i += 3) {
      Mapping mapping;
      mapping.destination = vs[i];
      mapping.source = vs[i + 1];
      mapping.range = vs[i + 2];
      m.push_back(mapping);
    }
  };

  auto sep = std::string{"\n\n"};
  while (pos < input_str.length()) { // string::npos) {
    auto n = input_str.find(sep, pos);
    if (n == string::npos) {
      n = input_str.length();
    }
    // std::cout << "[" << input_str.substr(pos, n - pos) << "]" << std::endl;
    auto s = input_str.substr(pos, n - pos);
    if (s.rfind("seeds:", 0) != string::npos) {
      auto num = std::regex("([0-9]+)");
      std::for_each(std::sregex_token_iterator(s.begin(), s.end(), num, 1),
                    std::sregex_token_iterator(), [&seeds](auto i) {
                      seeds.push_back(stoull(i.str().c_str()));
                    });

    } else if (s.rfind("seed-to-soil", 0) != string::npos) {
      // std::cout << "add to map : seed-to-soil" << endl;
      add_mapping(s, ref(seed_to_soil));
    } else if (s.rfind("soil-to-fertilizer", 0) != string::npos) {
      add_mapping(s, ref(soil_to_fertilizer));
    } else if (s.rfind("fertilizer-to-water", 0) != string::npos) {
      add_mapping(s, ref(fertilizer_to_water));
    } else if (s.rfind("water-to-light", 0) != string::npos) {
      add_mapping(s, ref(water_to_light));
    } else if (s.rfind("light-to-temperature", 0) != string::npos) {
      add_mapping(s, ref(light_to_temperature));
    } else if (s.rfind("temperature-to-humidity", 0) != string::npos) {
      add_mapping(s, ref(temperature_to_humidity));
    } else if (s.rfind("humidity-to-location", 0) != string::npos) {
      add_mapping(s, ref(humidity_to_location));
    } else {
      cout << "unkown " << s << "\n";
    }
    //
    pos = n + 2;
  }
}

int main(void) {

  vector<unsigned long long> seeds{};
  vector<Mapping> seed_to_soil{};
  vector<Mapping> soil_to_fertilizer{};
  vector<Mapping> fertilizer_to_water{};
  vector<Mapping> water_to_light{};
  vector<Mapping> light_to_temperature{};
  vector<Mapping> temperature_to_humidity{};
  vector<Mapping> humidity_to_location{};
  ParseInput("input.txt", seeds, seed_to_soil, soil_to_fertilizer,
             fertilizer_to_water, water_to_light, light_to_temperature,
             temperature_to_humidity, humidity_to_location);

  auto find_value_from_mapping = [](unsigned long long source,
                                    const std::vector<Mapping> &mapping) {
    // cout << "source " << source << endl;

    for (auto i = size_t{0}; i < mapping.size(); i++) {
      auto m = mapping[i];
      // cout << "mapping.source " << m.source << endl;
      // cout << "mapping.destination " << m.destination << endl;
      // cout << "mapping.range " << m.range << endl;
      auto v = source - m.source;
      if (v < 0) {
        continue;
      }
      if (v <= m.range) {
        return m.destination + v;
      }
    }
    // if there is no mapping assume it is the same
    return source;
  };
  auto min_location = numeric_limits<unsigned long long>::max();
  for (auto i : seeds) {
    // std::cout << "seed " << i;
    auto soil = find_value_from_mapping(i, seed_to_soil);
    auto fertilizer = find_value_from_mapping(soil, soil_to_fertilizer);
    auto water = find_value_from_mapping(fertilizer, fertilizer_to_water);
    auto light = find_value_from_mapping(water, water_to_light);
    auto temperature = find_value_from_mapping(light, light_to_temperature);
    auto humidity =
        find_value_from_mapping(temperature, temperature_to_humidity);
    auto location = find_value_from_mapping(humidity, humidity_to_location);

    // std::cout << " location " << location << std::endl;
    cout << "seed [" << i << "] ";
    cout << "soil " << soil << " ";
    cout << "fertilizer " << fertilizer << " ";
    cout << "water " << water << " ";
    cout << "light " << light << " ";
    cout << "temperature " << temperature << " ";
    cout << "humidity " << humidity << " ";
    cout << "location " << location << "\n";
    min_location = min(location, min_location);
  }
  std::cout << min_location << std::endl;
  return 0;
}
