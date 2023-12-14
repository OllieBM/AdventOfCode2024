
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

map<unsigned long long, unsigned long long> add_to_map(std::string text) {
  map<unsigned long long, unsigned long long> m{};
  auto nums = std::regex("([0-9]+)");
  std::vector<unsigned long long> vs{};
  for (std::smatch sm; std::regex_search(text, sm, nums);) {
    vs.push_back(stoull(text.c_str()));
    text = sm.suffix();
  }

  auto source = vs[0];
  auto destination = vs[0];
  auto range = vs[0];
  for (unsigned long long i = 0; i < range; i++) {
    m.insert({source + i, destination + i});
  }
  return m;
}

void ParseInput(
    string filename, vector<unsigned long long> &seeds,
    map<unsigned long long, unsigned long long> &seed_to_soil,
    map<unsigned long long, unsigned long long> &soil_to_fertilizer,
    map<unsigned long long, unsigned long long> &fertilizer_to_water,
    map<unsigned long long, unsigned long long> &water_to_light,
    map<unsigned long long, unsigned long long> &light_to_temperature,
    map<unsigned long long, unsigned long long> &temperature_to_humidity,
    map<unsigned long long, unsigned long long> &humidity_to_location) {

  auto ifs = ifstream(filename);
  if (!ifs.is_open()) {
    cout << "Error opening file.\n";
    exit(-1);
  }

  std::stringstream buffer;
  buffer << ifs.rdbuf();
  string input_str = buffer.str();
  auto pos = size_t{0};

  auto add_to_map = [](std::string text,
                       map<unsigned long long, unsigned long long> &m) {
    auto nums = std::regex("([0-9]+)");
    std::vector<unsigned long long> vs{};
    for (std::smatch sm; std::regex_search(text, sm, nums);) {
      vs.push_back(stoull(sm.str().c_str()));
      text = sm.suffix();
    }

    for (auto i = size_t{0}; i < vs.size(); i += 3) {
      auto destination = vs[i];
      auto source = vs[i + 1];
      auto range = vs[i + 2];
      for (unsigned long long j = 0; j < range; j++) {
        m.insert({source + j, destination + j});
      }
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
      add_to_map(s, ref(seed_to_soil));
    } else if (s.rfind("soil-to-fertilizer", 0) != string::npos) {
      add_to_map(s, ref(soil_to_fertilizer));
    } else if (s.rfind("fertilizer-to-water", 0) != string::npos) {
      add_to_map(s, ref(fertilizer_to_water));
    } else if (s.rfind("water-to-light", 0) != string::npos) {
      add_to_map(s, ref(water_to_light));
    } else if (s.rfind("light-to-temperature", 0) != string::npos) {
      add_to_map(s, ref(light_to_temperature));
    } else if (s.rfind("temperature-to-humidity", 0) != string::npos) {
      add_to_map(s, ref(temperature_to_humidity));
    } else if (s.rfind("humidity-to-location", 0) != string::npos) {
      add_to_map(s, ref(humidity_to_location));
    } else {
      cout << "unkown " << s << "\n";
    }
    //
    pos = n + 2;
  }
}

int main(void) {

  vector<unsigned long long> seeds{};
  map<unsigned long long, unsigned long long> seed_to_soil{};
  map<unsigned long long, unsigned long long> soil_to_fertilizer{};
  map<unsigned long long, unsigned long long> fertilizer_to_water{};
  map<unsigned long long, unsigned long long> water_to_light{};
  map<unsigned long long, unsigned long long> light_to_temperature{};
  map<unsigned long long, unsigned long long> temperature_to_humidity{};
  map<unsigned long long, unsigned long long> humidity_to_location{};
  ParseInput("input.txt", seeds, seed_to_soil, soil_to_fertilizer,
             fertilizer_to_water, water_to_light, light_to_temperature,
             temperature_to_humidity, humidity_to_location);

  auto find_in_map_or_default =
      [](unsigned long long seed,
         std::map<unsigned long long, unsigned long long> m) {
        auto it = m.find(seed);
        if (it != m.end()) {
          return it->second;
        }
        return seed;
      };

  auto min_location = numeric_limits<unsigned long long>::max();
  for (auto i : seeds) {
    std::cout << "seed " << i;
    auto soil = find_in_map_or_default(i, seed_to_soil);
    auto fertilizer = find_in_map_or_default(soil, soil_to_fertilizer);
    auto water = find_in_map_or_default(fertilizer, fertilizer_to_water);
    auto light = find_in_map_or_default(water, water_to_light);
    auto temperature = find_in_map_or_default(light, light_to_temperature);
    auto humidity =
        find_in_map_or_default(temperature, temperature_to_humidity);
    auto location = find_in_map_or_default(humidity, humidity_to_location);

    std::cout << " location " << location << std::endl;
    // cout << "seed [" << i << "] ";
    // cout << "soil " << soil << " ";
    // cout << "fertilizer " << fertilizer << " ";
    // cout << "water " << water << " ";
    // cout << "light " << light << " ";
    // cout << "temperature " << temperature << " ";
    // cout << "humidity " << humidity << " ";
    // cout << "location " << location << "\n";
    min_location = min(location, min_location);
  }
  std::cout << min_location << std::endl;
  return 0;
}
