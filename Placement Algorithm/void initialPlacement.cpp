void initialPlacement(int xLimit)
{
	map<string, node> ::iterator itrNode;
	map<int, row> ::iterator itrRow=rowId.begin();
	int totalWidth = 0, rowWidth = 0, numNodes =0, cnt=0,count=0, xCord=xLimit,yCord=0;
	float numCellsPerRow = 0;
	for(itrNode = nodeId.begin(); itrNode != nodeId.end(); ++itrNode)
		{
			if(itrNode->second.terminal == 0 && itrNode->second.height <= 16)
			{
				totalWidth += itrNode->second.width;
				numNodes++;
			}
		}
		totalWidth = totalWidth + numberNodes;
		rowWidth = totalWidth/rowId.size();
		RowWidth = rowWidth;
		numberCellsPerRow = ceil(float (numberNodes)/rowId.size());
		numberCellsPerRow = numberCellsPerRow-1;
		itrNode = nodeId.begin();
	while( itrNode != nodeId.end())
	{
		if(itrRow != rowId.end())
		{
			if(itrNode->second.terminal == 0)
			{
				if(itrNode->second.height == itrRow->second.height)
				{
					if(cnt < (numCellsPerRow))
			  		{
					itrNode->second.yCoordinate = itrRow->second.coOrdinate;
					itrNode->second.xCoordinate = xCord;
					xCord += itrNode->second.width+1;					
					cnt++;										
						if(cnt == numberCellsPerRow)
						{						
							cnt = 0;
							xCord = xLimit;
							++itrRow;	
						}
					}
				}
			}
		}
 		++itrNode;
	}
createRowToCellMap();
}