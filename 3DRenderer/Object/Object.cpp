#include "Object.h"

Object::Object()
{
}

Object::~Object()
{
	assert(m_initState == eObjectInitState::Destroyed && ("Object tried to delete without calling Destroy()"));
	m_initState = eObjectInitState::Deleted;
}
void Object::Initialize()
{
	assert(m_initState == eObjectInitState::Uninitialized && ("Object is corrupt, or has already called Initialize()"));
	assert(m_owner && ("Object initialized without an owner")); // Make sure we have an owner set
	m_initState = eObjectInitState::Initialized;
}
void Object::Destroy()
{
	assert(m_initState == eObjectInitState::Initialized && ("Object is corrupt, or has already called Destroy(), or never called Initialize()"));

	// Make sure we aren't deleted until this stack frame ends at the earliest
	auto self = shared_from_this(); //grab shared pointer to self

	// Remove from owner's child array
	assert(m_owner->GetInitState() >= eObjectInitState::Initialized && ("Owner was never initialized"));
	if (m_owner && m_owner != this) // NOTE: We only require that the owner *was ever* initialized
	{
		m_owner->RemoveChild(m_childIdx); //"take me out of your list of children"
	}
	m_owner = nullptr;
	m_childIdx = -1; //-1 means "invalid"
	m_initState = eObjectInitState::Destroyed; // Make sure we are marked invalid prior to destroying children

	// Destroy children and mark us destroyed
	DestroyAllChildren();
}
void Object::SetOwner(tOwner in_owner)
{
	bool isRootOwner = in_owner == this;
	bool isValidOwner = isRootOwner || IsValid(in_owner);
	assert(isValidOwner && ("Attempted to set invalid owner"));
	if (in_owner != m_owner)
	{
		if (m_owner) // Remove child from old owner
		{
			m_owner->RemoveChild(m_childIdx);
		}
		m_owner = in_owner; // Set new owner
		if (m_owner && !isRootOwner) // Add child to new owner
		{
			m_childIdx = m_owner->AddChild(shared_from_this());
		}
	}
}
void Object::DestroyAllChildren()
{
	auto children = m_children; // Make a copy of children, for safety
	for (auto child : children)
	{
		// Destroy object
		bool isValidChild = child->IsInitialized();
		assert(isValidChild && ("Child object is invalid"));
		if (isValidChild)
		{
			child->Destroy(); // Children will remove themselves from m_children automatically upon destruction
		}
		assert(child->IsDestroyed() && ("Child object was not marked 'destroyed' - check for a missing an parent Destroy() call"));
	}
}
int32_t Object::AddChild(tChild in_child)
{
	// Add the child to the child array
	in_child->m_childIdx = (int32_t)m_children.size();
	m_children.push_back(in_child);
	return in_child->m_childIdx;
}
void Object::RemoveChild(int32_t in_childIdx)
{
	// Swap last element in array with the child element, then pop the last element ("swap and pop")
	m_children[in_childIdx] = m_children.back();
	m_children[in_childIdx]->m_childIdx = in_childIdx;
	m_children.pop_back();
}
