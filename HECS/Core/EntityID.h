class EntityID
{
public:
	explicit EntityID(int id) : m_id(id) {}
	int GetID() const { return m_id; }

	bool operator==(const EntityID& other) const { return m_id == other.m_id; }
	bool operator!=(const EntityID& other) const { return m_id != other.m_id; }
	bool operator<(const EntityID& other) const { return m_id < other.m_id; }

private:
	int m_id;
};
