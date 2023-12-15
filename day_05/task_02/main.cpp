
#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <numeric>
#include <regex>
#include <sstream>
#include <vector>

#include <future>

using namespace std;

struct Mapping {
  unsigned long long source;
  unsigned long long destination;
  unsigned long long range;
};

bool operator<(const Mapping &x, const Mapping &y) {
  return y.source > x.source;
}

void ParseInput(
    string filename,
    vector<std::pair<unsigned long long, unsigned long long>> &seeds,
    vector<Mapping> &seed_to_soil, vector<Mapping> &soil_to_fertilizer,
    vector<Mapping> &fertilizer_to_water, vector<Mapping> &water_to_light,
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
      std::vector<unsigned long long> temp;
      std::for_each(std::sregex_token_iterator(s.begin(), s.end(), num, 1),
                    std::sregex_token_iterator(), [&temp](auto i) {
                      temp.push_back(stoull(i.str().c_str()));
                    });

      for (auto i = size_t{0}; i < temp.size(); i += 2) {
        auto s = temp[i];
        auto e = temp[i + 1];
        seeds.push_back({s, e});
      }
    } else if (s.rfind("seed-to-soil", 0) != string::npos) {
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
      // assert here
      cout << "unkown\n[" << s << "]\n";
    }
    //
    pos = n + 2;
  }
}

int main(void) {

  const auto start = std::chrono::system_clock::now();
  const std::time_t t_c = std::chrono::system_clock::to_time_t(start);
  std::cout << "start time: " << std::ctime(&t_c) << std::endl;

  vector<std::pair<unsigned long long, unsigned long long>> seeds{};
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

  std::sort(seeds.begin(), seeds.end(),
            [](auto x, auto y) -> bool { return x.first < y.first; });
  std::sort(seed_to_soil.begin(), seed_to_soil.end());
  std::sort(soil_to_fertilizer.begin(), soil_to_fertilizer.end());
  std::sort(fertilizer_to_water.begin(), fertilizer_to_water.end());
  std::sort(water_to_light.begin(), water_to_light.end());
  std::sort(light_to_temperature.begin(), light_to_temperature.end());
  std::sort(temperature_to_humidity.begin(), temperature_to_humidity.end());
  std::sort(humidity_to_location.begin(), humidity_to_location.end());

  // debug
  // std::cout << " seeds count \t" << seeds.size() << std::endl;
  // std::cout << " seed_to_soil count \t" << seed_to_soil.size() << std::endl;
  // std::cout << " soil_to_fertilizer count \t" << soil_to_fertilizer.size()
  //           << std::endl;
  // std::cout << " fertilizer_to_water count \t" << fertilizer_to_water.size()
  //           << std::endl;
  // std::cout << " water_to_light count \t" << water_to_light.size() <<
  // std::endl; std::cout << " light_to_temperature count \t" <<
  // light_to_temperature.size()
  //           << std::endl;
  // std::cout << " temperature_to_humidity count \t"
  //           << temperature_to_humidity.size() << std::endl;
  // std::cout << " humidity_to_location count \t" <<
  // humidity_to_location.size()
  //           << std::endl;

  auto find_value_from_mapping = [](unsigned long long source,
                                    const std::vector<Mapping> &mapping) {
    auto it =
        upper_bound(mapping.begin(), mapping.end(), source,
                    // element value
                    [](unsigned long long value, const Mapping &i) -> bool {
                      return value < (i.source + i.range);
                    });

    if (it != mapping.end()) {

      if (it->source > source) {
        return source;
      }
      return it->destination + (source - it->source);
    }

    return source;
  };
  std::vector<future<unsigned long long>> futures{};
  for (auto i : seeds) {

    futures.push_back(std::async(std::launch::async, [=]() {
      auto min_location = numeric_limits<unsigned long long>::max();
      for (auto seed = i.first; seed <= i.first + i.second; seed++) {

        auto soil = find_value_from_mapping(seed, seed_to_soil);

        auto fertilizer = find_value_from_mapping(soil, soil_to_fertilizer);
        auto water = find_value_from_mapping(fertilizer, fertilizer_to_water);
        auto light = find_value_from_mapping(water, water_to_light);
        auto temperature = find_value_from_mapping(light, light_to_temperature);
        auto humidity =
            find_value_from_mapping(temperature, temperature_to_humidity);
        auto location = find_value_from_mapping(humidity, humidity_to_location);

        // std::cout << " location " << location << std::endl;
        // auto ss = std::stringstream{};
        // ss << "seed [" << seed << "] ";
        // ss << "soil " << soil << " ";
        // ss << "fertilizer " << fertilizer << " ";
        // ss << "water " << water << " ";
        // ss << "light " << light << " ";
        // ss << "temperature " << temperature << " ";
        // ss << "humidity " << humidity << " ";
        // ss << "location " << location << "\n";
        // std::cout << ss.str();
        min_location = min(location, min_location);
      }
      return min_location;
    }));
  }

  auto min_location = numeric_limits<unsigned long long>::max();
  for (auto &&fut : futures) {
    min_location = min(fut.get(), min_location);
  }

  std::cout << "----\nsolution :" << min_location << std::endl;
  std::cout << "----\n" << std::endl;

  const auto end = std::chrono::system_clock::now();
  const auto delta =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
  const std::time_t t_e = std::chrono::system_clock::to_time_t(end);

  std::cout << "end time: " << std::ctime(&t_e) << std::endl;
  std::cout << "difference in milliseconds: " << (delta.count()) << std::endl;

  return 0;
}
