#ifndef RESOURCE_HOLDER_HPP
#define RESOURCE_HOLDER_HPP

#include "SFML/Graphics.hpp"
#include "SFML/Audio/SoundBuffer.hpp"
#include <map>

template<typename Resource, typename ID>
class ResourceHolder
{
	std::map<ID, Resource*> fResources;
public:
	ResourceHolder();

	void load(const std::string& FileName, ID id);

	template<typename Param>
	void load(const std::string& FileName, ID id, Param SecondParam);

	Resource& get(ID id);
	const Resource& get(ID id) const;

	~ResourceHolder();
};


#include "ResourceHolder.inl"
#endif //RESOURCE_HOLDER_HPP