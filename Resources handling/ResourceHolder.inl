#include "ResourceHolder.hpp"
#include <exception>
#include <assert.h>
#include <iostream>

template<typename Resource, typename ID>
ResourceHolder<Resource, ID>::ResourceHolder()
	:fResources()
{
}

template<typename Resource, typename ID>
void ResourceHolder<Resource, ID>::load(const std::string& FileName, ID id)
{
	Resource* res = new Resource();
	if (!res->loadFromFile(FileName))
	{
		throw std::runtime_error("Unable to load file: " + FileName);
	}

	auto inserted = fResources.insert(std::make_pair(id, res));
	assert(inserted.second);
}

template<typename Resource, typename ID>
Resource & ResourceHolder<Resource, ID>::get(ID id)
{
	auto found = fResources.find(id);
	assert(found != fResources.end());
	return *found->second;
}

template<typename Resource, typename ID>
const Resource & ResourceHolder<Resource, ID>::get(ID id) const
{
	auto found = fResources.find(id);
	assert(found != fResources.end());
	return *found->second;
}

template<typename Resource, typename ID>
ResourceHolder<Resource, ID>::~ResourceHolder()
{
	for (auto res : fResources)
	{
		delete res.second;
	}
}

template<typename Resource, typename ID>
template<typename Param>
void ResourceHolder<Resource, ID>::load(const std::string & FileName, ID id, Param SecondParam)
{
	Resource* res = new Resource();
	if (!res->loadFromFile(FileName, SecondParam))
	{
		throw std::runtime_error("Unable to load file: " + FileName);
	}
	auto inserted = fResources.insert(std::make_pair(id, res));
	assert(inserted.second);
}
