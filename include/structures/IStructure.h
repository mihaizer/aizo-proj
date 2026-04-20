class IStructure
{
public:
    virtual ~IStructure() = default;

    virtual int size() const = 0;
    virtual bool isEmpty() const = 0;

    virtual int &at(int index) = 0;
    virtual const int &at(int index) const = 0;

    virtual void pushBack(const int &value) = 0;
    virtual void clear() = 0;
    virtual void swap(int i, int j) = 0;
};
