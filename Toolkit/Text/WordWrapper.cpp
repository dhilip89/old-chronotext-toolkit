#include "WordWrapper.h"

using namespace std;

float WordWrapper::wrap(XFont *font, wstring *text)
{
	size = 0;
    int length = text->size();
    
    int lineIndex = 0;
    int lineLength = 0;
    wchar_t c = 0;
    float x = 0;
    float max = 0;
    
    for (int i = 0; i < length; i++)
    {
		wchar_t lastC = c;
		c = text->at(i);
        
		if (c == '\r' || c == '\n') // WIN, UNIX, OSX
		{
			if (lastC == '\r' && c == '\n') // WIN
			{
				lineIndex = i + 1;
			}
			else
			{
				max = fmaxf(max, x);
                addLine(text, lineIndex, lineLength);
				lineIndex = i + 1;
				lineLength = 0;
				x = 0;
			}
            
			continue;
		}
        
		lineLength++;
		x += font->getCharWidth(c);
    }
    
    if (lineLength > 0)
    {
		max = fmaxf(max, x);
        addLine(text, lineIndex, lineLength);
    }
    
    return max;
}

void WordWrapper::wrap(XFont *font, wstring *text, float width)
{
	size = 0;
    int length = text->size();
    
    int spaceIndex = -2; // lastToken = 2
    int wordIndex = -2; // lastToken = 3
    int hyphenIndex = -2;
    int brokenIndex = -2;
    int lastToken = 0;
    bool newLine = true;
    int lineIndex = 0;
    int lineLength = 0;
    wchar_t c = 0;
    float x = 0;
    
    for (int i = 0; i < length; i++)
    {
		wchar_t lastC = c;
		c = text->at(i);
        wchar_t nextC = (i + 1) < length ? text->at(i + 1) : 0;
        
        if (c == '\r' || c == '\n') // WIN, UNIX, OSX
		{
			if (lastC == '\r' && c == '\n') // WIN
			{
				lineIndex = i + 1;
			}
			else
			{
				if (brokenIndex != i - 1)
				{
                    addLine(text, lineIndex, lineLength);
				}
                
				lineIndex = i + 1;
				lineLength = 0;
				x = 0;
				lastToken = 0;
				newLine = true;
			}
			continue;
		}
        
        if (c == ' ' && (lastC != ' ' || lineIndex == i))
		{
			spaceIndex = i;
			lastToken = 2;
		}
		else if (c != ' ' && (lastC == ' ' || lineIndex == i || hyphenIndex == i - 1))
		{
			wordIndex = i;
			lastToken = 3;
		}
		else if (wordIndex != i && c == '-' && lastC != '-' && nextC != 0 && nextC != ' ' && nextC != '\r' && nextC != '\n')
		{
			hyphenIndex = i;
		}
        
        float charWidth = font->getCharWidth(c);
        
		if (x + charWidth >= width)
		{
			if (lastToken == 2) // LINES ENDING WITH SPACES
			{
				while (spaceIndex + 1 < length && text->at(spaceIndex + 1) == ' ')
				{
					spaceIndex++;
				}
				if (newLine || wordIndex >= lineIndex)
				{
                    addLine(text, lineIndex, spaceIndex - lineIndex);
				}
                
				lineIndex = spaceIndex + 1;
				i = brokenIndex = lineIndex - 1;
			}
			else if (lastToken == 3 && hyphenIndex >= lineIndex && hyphenIndex < wordIndex) // HYPHEN-BREAK
			{
                addLine(text, lineIndex, wordIndex - lineIndex);
				lineIndex = wordIndex;
				i = lineIndex - 1;
			}
			else if (lastToken == 3 && spaceIndex >= lineIndex) // WORD-BREAK
			{
                addLine(text, lineIndex, wordIndex - lineIndex);
				lineIndex = wordIndex;
				i = lineIndex - 1;
			}
			else // LONG-LINE-BREAK
			{
				lineLength += lineLength == 0 ? 1 : 0; // FOR EXTREMELY-NARROWED-WIDTH CASES
                addLine(text, lineIndex, lineLength);
				lineIndex = lineIndex + lineLength;
				i = brokenIndex = lineIndex - 1;
			}
            
			c = text->at(i);
			lineLength = 0;
			x = 0;
			lastToken = 0;
			newLine = false;
		}
		else if (!newLine && x == 0 && c == ' ') // ONLY SPACES AT THE BEGINNING OF NEW-LINE ARE ENABLED
		{
			lineIndex++;
		}
		else
		{
			lineLength++;
			x += charWidth;
		}
    }
    
    if (lineLength > 0)
    {
        addLine(text, lineIndex, lineLength);
    }
}

void WordWrapper::addLine(wstring *text, int offset, int length)
{
    /*
     * TRIMMING TRAILING SPACES FROM DISPLAY
     */
    while (length > 0 && text->at(offset + length - 1) == ' ')
    {
		length--;
	}
    
	offsets.push_back(offset);
    lengths.push_back(length);
	size++;
}
