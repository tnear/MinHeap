// Templated MinHeap which includes test cases

template <typename T>
class MinHeap
{
public:
    auto& insert(const T &value)
    {
        // insert at back of array
        m_data.push_back(value);

        // move value to proper location
        heapUp();

        // builder pattern: allow chaining insert operations
        return *this;
    }

    T extractMin()
    {
        if (m_data.empty())
            throw std::exception("Unsupported extract from empty heap");

        // swap min value with back then remove
        const T minValue = m_data.front();
        std::swap(m_data.front(), m_data.back());
        m_data.pop_back();

        // re-establish heap invariant
        heapDown();

        return minValue;
    }

    // breadth-first is built-in to the heap structure, so a breadth-first traversal means simply returning array
    std::vector<T> BFS() const
    {
        return m_data;
    }

private:

    void heapUp()
    {
        // move element upward in synthetic tree (array) until element is >= parent
        auto currIdx = m_data.size() - 1;
        while (currIdx > 0)
        {
            auto parentIdx = getParentIdx(currIdx);
            if (m_data[currIdx] < m_data[parentIdx]) // move up
            {
                std::swap(m_data[currIdx], m_data[parentIdx]);
                currIdx = parentIdx; // update location
            }
            else
                break; // found proper location
        }
    }

    void heapDown()
    {
        // the new element is not in its proper location--
        // continue swapping with smallest child
        int currIdx = 0;

        while (1)
        {
            const auto leftIdx = getLeftIdx(currIdx);
            const auto rightIdx = getRightIdx(currIdx);
            auto idxToSwap = -1;

            if (leftIdx >= m_data.size() && rightIdx >= m_data.size())
                break; // leaf
            else if (leftIdx < m_data.size() && rightIdx >= m_data.size())
                idxToSwap = leftIdx; // only left child exists
            else if (leftIdx >= m_data.size() && rightIdx < m_data.size())
                idxToSwap = rightIdx; // only right child exists
            else
                idxToSwap = m_data[leftIdx] <= m_data[rightIdx] ? leftIdx : rightIdx; // both exist, take smaller

            // swap downward if heap invariant is not yet achieved
            if (m_data[currIdx] > m_data[idxToSwap])
                std::swap(m_data[currIdx], m_data[idxToSwap]);
            else
                break;

            currIdx = idxToSwap;
        }
    }

    static int getParentIdx(int idx)
    {
        return (idx - 1) / 2;
    }

    static std::size_t getLeftIdx(int idx)
    {
        return idx * 2 + 1;
    }

    static std::size_t getRightIdx(int idx)
    {
        return idx * 2 + 2;
    }

    std::vector<T> m_data;
};


void testMinHeapDifferentDataTypes()
{
    // integer heap
    MinHeap<int> h;
    assert(h.BFS() == std::vector<int>({}));

    h.insert(1);
    assert(h.BFS() == std::vector<int>({1}));

    h.insert(3).insert(5);
    assert(h.BFS() == std::vector<int>({ 1, 3, 5 }));

    h.insert(2);
    assert(h.BFS() == std::vector<int>({ 1, 2, 5, 3 }));

    h.insert(4).insert(6).insert(7);
    assert(h.BFS() == std::vector<int>({ 1, 2, 5, 3, 4, 6, 7 }));

    h.insert(0);
    assert(h.BFS() == std::vector<int>({ 0, 1, 5, 2, 4, 6, 7, 3 }));

    // double heap
    MinHeap<double> d;
    d.insert(1.1).insert(3.3).insert(5.5).insert(2.2).insert(4.4).insert(6.6).insert(7.7).insert(0.0);
    assert(d.BFS() == std::vector<double>({ 0.0, 1.1, 5.5, 2.2, 4.4, 6.6, 7.7, 3.3 }));
    
    // string heap
    MinHeap<std::string> s;
    s.insert("a").insert("d").insert("c").insert("b");
    std::vector<std::string> expOrder = { "a", "b", "c", "d" };
    s.extractMin();
    expOrder = { "b", "d", "c" };
    assert(s.BFS() == expOrder);
}

void testExtractMin()
{
    MinHeap<int> h;
    try
    {
        h.extractMin();
        assert(false);
    }
    catch (std::exception)
    {
        assert(true);
    }

    assert(h.BFS().empty());

    h.insert(1);
    assert(h.extractMin() == 1);
    assert(h.BFS().empty());

    h.insert(1).insert(2).insert(3).insert(4).insert(5).insert(6).insert(7);

    assert(h.extractMin() == 1);
    assert(h.BFS() == std::vector<int>({2, 4, 3, 7, 5, 6 }));

    assert(h.extractMin() == 2);
    assert(h.BFS() == std::vector<int>({ 3, 4, 6, 7, 5 }));

    assert(h.extractMin() == 3);
    assert(h.BFS() == std::vector<int>({  4, 5, 6, 7 }));

    assert(h.extractMin() == 4);
    assert(h.BFS() == std::vector<int>({ 5, 7, 6 }));

    assert(h.extractMin() == 5);
    assert(h.BFS() == std::vector<int>({ 6, 7 }));

    assert(h.extractMin() == 6);
    assert(h.BFS() == std::vector<int>({ 7 }));

    assert(h.extractMin() == 7);
    assert(h.BFS() == std::vector<int>({ }));
}
