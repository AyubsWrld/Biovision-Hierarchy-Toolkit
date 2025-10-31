#include <cstddef>
#include <iostream>
#include <vector>
#include <cstdint>
#include <algorithm>
#include <cstdlib>
#include <optional>
#include <string>
#include <utility>
#include <fstream>
#include <stack>

enum class EJointTag
{
  EJT_UNDEF, 
  EJT_ROOT, 
  EJT_JOINT, 
  EJT_END, 
};

struct FOffset
{
  float x,y,z;
};

struct FBVHNode
{
  FBVHNode* Parent{};
  std::vector<FBVHNode*> Children;
  EJointTag Type; 
};

class UBiovisionHierarchyFile
{
  std::vector<EJointTag> m_hierarchy;
  const uint32_t m_motion_offset; 

public:
  UBiovisionHierarchyFile(std::ifstream& f);
  uint32_t GetMotionOffset() const; 
};

[[maybe_unused]] std::optional<std::pair<std::string, std::string>> ParseArg( const std::string& str , char delim = '=' )
{
  std::string::const_iterator begin{ str.begin() };
  std::string::const_iterator end{ str.end() };
  if( std::string::const_iterator it {std::find(begin, end, delim)} ; it != end )
  {
    std::ptrdiff_t idx{ it - begin };
    std::pair<std::string, std::string> V{str.substr(0, idx), str.substr(idx + 1, str.size())};
    return V;
  }
  return {};
}


[[maybe_unused]] std::vector<std::string> tokenize_line(std::string line)
{
  std::ptrdiff_t diff{}; 
  std::string::size_type slow{}; 
  std::string::size_type fast{}; 
  for(auto i : line)
  {
    if(line[fast] == ' ' && slow != fast)
    {
      std::cout << line.substr(slow, fast) << std::endl;
      fast = slow;
    }
    fast++;
  }
  return {}; 
}


/* Read stripped whitespace assumes no fully tabbed lines? */ 

std::string trim_whitespace(std::string value)
{
  std::string::size_type idx{}; 
  while(value[idx] == '\t') idx++ ; 
  return value.substr(idx, value.size()); 
}


int main (int argc, char *argv[]) 
{
  if(argc <= 1)
  {
    std::cout << "Usage: ./transform --input=file.bvh" << std::endl;
    return EXIT_FAILURE; 
  }

  auto Values{ ParseArg(argv[1]) }; 
  if(!Values.has_value())
    return EXIT_FAILURE; 

  std::ifstream file{Values->second, std::ios_base::in};

  if(!file.is_open())
  {
    std::cout << "Could not open file" << std::endl;
    return EXIT_FAILURE; 
  }
  std::string buffer; 

  std::stack<char> Tree; 

  int tabs{}; 
  while(std::getline(file, buffer))
  {
    std::string line = trim_whitespace(buffer);
    char current = line[0];
    if(current == '{')
    {
      std::cout << std::string(tabs,' ') << current << std::endl;
      tabs++; 
      Tree.push('{');
    }
    else if(current == '}')
    {
      tabs--;
      std::cout << std::string(tabs,' ') << current << std::endl;
      Tree.push('}');
    }
    else 
    {
      tokenize_line(line);
    }
  }


  return EXIT_SUCCESS;
}
