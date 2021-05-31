#pragma once

#include <memory>
#include <cassert>
#include <vector>
#include <functional>

//--- eObjectInitState ---//
enum class eObjectInitState
{
	//constructor called
	Uninitialized,
	//initialize called
	Initialized,
	//destroy called
	Destroyed,
	//destructor called
	Deleted,
};

//--- Object ---//
class Object : public std::enable_shared_from_this<Object> //puts the control block inside the object; "public" makes parent's functions available -- you want public 99.9999999999999%, srsly use public (also this implies single public inheritance)
{
public: //everything in this block is tagged "public": if someone has a pointer to you, they can use . or -> to access/use these
	using tSharedPtr = std::shared_ptr<Object>; //"using" = "typedef" in C++11, aliases the type to we don't have to type it constantly
	using tOwner = Object * ;
	using tChild = tSharedPtr;
	using tChildren = std::vector<tChild>;

	// Virtual interface
	Object(); // can't call virtual functions because it doesn't even exist yet - NEVER virtual!
	virtual ~Object();
	virtual void Initialize(); //"this is entering the simulation"
	virtual void Destroy(); //"this is exiting the simulation" (this being "virtual" makes it a class that can be INHERITED from)
	//contract: you are always allowed to call Destroy() on an Initialized object, no matter what else is going on

	//"virtual" = when you have a shared pointer to an Object, it could be any child class - they all look like Object pointers to the machine, though they are actually players/enemies/etc. The most-derived class from Object (creature, say) will have a "virtual function table" that contains pointers to the MOST DERIVED versions of every function you need to be able to use from its various ancestors
			// aesthetically, you almost always want your concrete types that you pass into make_shared to be the leaves of your inheritance tree
		//use "override" on a leaf subclass's overwritten function of a parent function

	// Initialization state
	eObjectInitState GetInitState() const { return m_initState; } //"const" here means this is an ACCESSOR - can't change anything, basically an inspector
	bool IsInitialized() const { return m_initState == eObjectInitState::Initialized; }
	bool IsDestroyed() const { return m_initState == eObjectInitState::Destroyed; }

	// Ownership - everything MUST have an unbroken chain of ownership to a root, or it will assert
	void SetOwner(tOwner in_owner); // Must be called prior to Initialize()
	tOwner GetOwner() const { return m_owner; }
	void DestroyAllChildren(); //will handle the destruction of allllll the related objects

	// Spawn functions
	template <typename tObjectType, typename... Args>
	static std::shared_ptr<tObjectType> SpawnRoot(Args... args);
	template <typename tObjectType, typename... Args>
	static std::shared_ptr<tObjectType> Spawn(tSharedPtr in_owner, Args... args);
	template <typename tObjectType, typename... Args>
	static std::shared_ptr<tObjectType> SpawnWithSetup(tSharedPtr in_owner, const std::function<void(tObjectType*)>& in_setupFunc, Args... args);

protected: // you can get to this stuff from a child class
	// Helper to get derived shared_ptr from this
	template <typename tDerived>
	std::shared_ptr<tDerived> shared_from_base()
	{
		return std::static_pointer_cast<tDerived>(shared_from_this());
	}

private: // these can't be grabbed from outside -- these are for you to use internally to implement this class's functions
	// Child management
	int32_t AddChild(tChild in_child); // in_ prefix = input variable
	void RemoveChild(int32_t in_childIdx);

	// State
	tOwner m_owner = nullptr; // m_ prefix = member variable
	tChildren m_children;
	int32_t m_childIdx = -1;
	eObjectInitState m_initState = eObjectInitState::Uninitialized;
};

// Validity-check free function
static bool IsValid(Object* in_obj)
{
	return in_obj && in_obj->IsInitialized();
}
static bool IsValid(const std::shared_ptr<Object>& in_obj)
{
	return IsValid(in_obj.get());
}

// Spawn functions
template <typename tObjectType>
std::shared_ptr<tObjectType> SetupObject(const std::shared_ptr<tObjectType>& in_obj, Object::tSharedPtr in_owner, const std::function<void(tObjectType*)>& in_setupFunc = {})
{
	// Set object's owner
	in_obj->SetOwner(in_owner.get());

	// Call setup function prior to initialization (if provided)
	if (in_setupFunc)
	{
		in_setupFunc(in_obj.get());
	}

	// Initialize the object as the last step
	in_obj->Initialize();

	// Return the initialized object
	return in_obj;
}

template <typename tObjectType, typename... Args>
std::shared_ptr<tObjectType> Object::Spawn(tSharedPtr in_owner, Args... args)
{
	// Allocate and construct the object
	auto obj = std::make_shared<tObjectType>(std::forward<Args>(args)...);

	// Setup, initialize, and return object
	return SetupObject(obj, in_owner);
}

template <typename tObjectType, typename... Args>
std::shared_ptr<tObjectType> Object::SpawnRoot(Args... args)
{
	// Allocate and construct the object
	auto obj = std::make_shared<tObjectType>(std::forward<Args>(args)...);

	// Setup, initialize, and return object
	return SetupObject(obj, obj);
}

template <typename tObjectType, typename... Args>
std::shared_ptr<tObjectType> Object::SpawnWithSetup(tSharedPtr in_owner, const std::function<void(tObjectType*)>& in_setupFunc, Args... args)
{
	// Allocate and construct the object
	auto obj = std::make_shared<tObjectType>(std::forward<Args>(args)...);

	// Setup, initialize, and return object
	return SetupObject(obj, in_owner, in_setupFunc);
}
