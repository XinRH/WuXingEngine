#pragma once
#include <string>

namespace WuXing
{
    class REFLECT {
    public:
        template<class T>
    	std::string  getMsgClassName(T& _a)
    	{
            //返回的类的全名
    	    std::string className = std::string(typeid(_a).name());
            cout<<className<<endl;
            //第一个空格索引
    		int firstSpaceIndex = className.find_first_of(" ") + 1;
            //最后一个空格索引
    		int lastSpaceIndex = className.find_last_of(" ");
            //类名长度
    		int nameLength = lastSpaceIndex - firstSpaceIndex - 2;
    		return className.substr(firstSpaceIndex, nameLength);
    	}
    };
} // namespace WuXing
