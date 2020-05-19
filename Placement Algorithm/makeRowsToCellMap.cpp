void makeRowsToCellMap()
{
	map<int, row> ::iterator itrRow;
	map<string, node> ::iterator itrNode;

	for(itrRow = rowId.begin(); itrRow != rowId.end(); ++itRow)
	{
		for(itrNode = nodeId.begin(); itrNode != nodeId.end(); ++itrNode)
		{
			if(itrNode->second.height <= 16)
			{
			if((itrRow->second.coOrdinate <= itrNode->second.yCoordinate) && ((itrRow->second.coOrdinate)+(itrRow->second.height)) >= itrNode->second.yCoordinate)
			{
				itrNode->second.setRowId(itRow->first);
				itrRow->second.setCellList(itNode->first);	
			}
			}
		}
	}
}