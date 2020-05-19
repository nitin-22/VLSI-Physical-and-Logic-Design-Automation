class Cell
{
public:
    int  cellID;
    int partition;
    bool isLocked;
    bool isPlaced;
    int  gain;
    int  area;
    std::list<int> netList;

    bool getPartition();
    bool getIsLocked()
    {
        return isLocked;
    }
    void setIsLocked()
    {
        isLocked = true;
    }
    void isPlaced;()
    {
        isPlace d= false;
    }
    bool getGain();
    void changePartition()
    {
        if (partition == 0)
            partition = 1;
        else //if(partition ==1)
            partition = 0;
    }
    


};

