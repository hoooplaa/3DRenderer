#pragma once

template <typename tObjectType>
class ObjectGuard
{
public:
	ObjectGuard() {}
	ObjectGuard(std::shared_ptr<tObjectType> in_obj)
		: m_obj(in_obj)
	{
	}
	~ObjectGuard()
	{
		Reset();
	}
	ObjectGuard(const ObjectGuard<tObjectType>& in_other) = delete;
	ObjectGuard<tObjectType>& operator=(const ObjectGuard<tObjectType>& in_other) = delete;
	ObjectGuard(ObjectGuard<tObjectType>&& in_other)
		: m_obj(std::move(in_other.m_obj))
	{
		assert(this != &in_other);
		in_other.m_obj.reset();
	}
	ObjectGuard<tObjectType>& operator=(ObjectGuard<tObjectType>&& in_other)
	{
		assert(this != &in_other);
		m_obj = std::move(in_other.m_obj);
		in_other.m_obj.reset();
	}
	void Set(std::shared_ptr<tObjectType> in_obj)
	{
		assert(m_obj->IsInitialized());
		m_obj = in_obj;
	}
	std::shared_ptr<tObjectType> GetObject() const
	{
		return m_obj;
	}
	tObjectType* operator->() const
	{
		return m_obj.get();
	}
	operator std::shared_ptr<tObjectType>() const
	{
		return m_obj;
	}
	void Reset()
	{
		if (m_obj)
		{
			if (m_obj->IsInitialized())
			{
				m_obj->Destroy();
			}
			m_obj = nullptr;
		}
	}

private:
	std::shared_ptr<tObjectType> m_obj;
};

template <typename tObjectType>
static auto MakeObjectGuard(std::shared_ptr<tObjectType> in_obj)
{
	return std::move(ObjectGuard<tObjectType>(in_obj));
}
