void calculatelayoutBoundarieslimit()
{
int xvalue,yvalue;
b.minX = 32767, b.maxX = -32768,b.minY = 32767, b.maxY = -32768;
	map<string, node> ::iterator itrNode;
        itrNode = nodeId.begin();
	while( itrNode != nodeId.end() )
	{
		if(itNode->second.terminal == 1)
		{
			xval = itNode->second.xCoordinate;
			yval = itNode->second.yCoordinate;
			if(xval < b.minX)
			{
				b.minX = xval;
			}
			if(xval > b.maxX)
			{
				b.maxX = xval;
			}
			if(xval < b.minY)
			{
				b.minY = yval;
			}
			if(xval > b.maxY)
			{
				b.maxY = yval;
			}
			
		}
                ++itrNode;
	}
}