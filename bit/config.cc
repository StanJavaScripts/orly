#include <bit/config.h>

#include <cassert>
#include <fstream>
#include <iterator>

#include <base/thrower.h>

using namespace Base;
using namespace Bit;
using namespace Bit::Config;
using namespace std;

void Append(vector<string> &lhs, vector<string> &&rhs) {
  lhs.reserve(lhs.size() + rhs.size());
  std::move(rhs.begin(), rhs.end(), back_inserter(lhs));
}

string get_mixin_dir(string dir) { return dir + "/mixins"; }

TConfig LoadMixin(TConfig &&config, std::string MixinName);


TConfig LoadConfig(TConfig &&config, TValidSections allowed_sections, const std::string &filename) {
  TJson json = TJson::Read(filename.c_str());
  if(allowed_sections.DefaultTargets) {
    Append(config.DefaultTargets, json.Extract<vector<string>>({"targets"});
  }
  if(allowed_sections.DefaultMixins) {
    Append(config.EnabledMixins, json.Extract<vector<string>>({"mixins"});
  }
}

/* Load project config.

Projects can specify
  - Mixins
  - Jobs to enable / disable
  - Targets to build

*/
TConfig LoadProjectConfig(const string &project_dir) {
  TConfig config;

  TJson file = LoadJson(project_dir + "bit.json");

  if(file.is_ob)

    addMixins(config, file);
  //

  return config;
}

TConfig Bit::TConfig::Load(const string &project_dir,
                           const string &user_dir,
                           const string &system_dir) {
  assert(LastNotSlash(project_dir));
  assert(LastNotSlash(user_dir));
  assert(LastNotSlash(system_dir));

  TConfig config = LoadProjectConfig(project_dir);

  TJson::Read(project_dir + "/bit.json");
  TJson::Read(user_dir + "/bit.json");
  TJson::Read(system_dir + "/bit.json");
  return TConfig();
}
