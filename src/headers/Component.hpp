#ifndef COMPONENT_HPP
#define COMPONENT_HPP

typedef unsigned long long int Uint64;
class GameObject;

class Component {
	public:
		Component() = delete;
		Component (const char* name) : m_parent(nullptr), m_name(name) {}
		virtual void start() = 0;
		virtual void update(Uint64) = 0;
		GameObject* setParent(GameObject* parent) { m_parent = parent; return m_parent; }
		GameObject* getParent() const { return m_parent; }
		const char* name() const { return m_name; }
	protected:
		GameObject* m_parent;
		const char* m_name;
};

#endif