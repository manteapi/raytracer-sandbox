#!/usr/bin/env python
#!/usr/bin/python

import os
import cairosvg

inputFilepath = os.path.abspath("./../raytracer-sandbox/buildDebug/coverage/coverageSummary.txt")
outputSVGFilepath = os.path.abspath("../.prettify-readme/coverage.svg")
outputPNGFilepath = os.path.abspath("../.prettify-readme/coverage.png")

#Open our coverage summary file
inputFile = open(inputFilepath, 'r')

#Count the number of lines
numberOfLines = len(inputFile.readlines())

#Put the cursor to the beginning of the file
inputFile.seek(0)

#Read the file until the interesting date (end of file - 3 lines)
cursorStarter = numberOfLines-3
for i in range(cursorStarter) :
    inputFile.readline()

#Store the interesting data for each line of interest (loi)
loi = inputFile.readline()

#Parse the lines of interests
loi_content = loi.split(" ")

#Get the coverage percentage
coverage_percentage = loi_content[3]

#Parse the string to only get the number
coverage_number = coverage_percentage.split("%")
coverage_value = (float)(coverage_number[0])

#Write this percentage in a svg file
outputSVGFile = open(outputSVGFilepath, 'w')

svgCode=""
svgCode='<svg xmlns="http://www.w3.org/2000/svg" width="90" height="20">'
svgCode=svgCode+'<linearGradient id="a" x2="0" y2="100%"><stop offset="0" stop-color="#bbb" stop-opacity=".1"/><stop offset="1" stop-opacity=".1"/></linearGradient>'
svgCode=svgCode+'<rect rx="3" width="90" height="20" fill="#555"/>'
if coverage_value<75 :
    svgCode=svgCode+'<rect rx="3" x="55" width="35" height="20" fill="#ef1a1a"/>'
    svgCode=svgCode+'<path fill="#ef1a1a" d="M55 0h4v20h-4z"/>'
elif coverage_value>=75 and coverage_value<=90 :
    svgCode=svgCode+'<rect rx="3" x="55" width="35" height="20" fill="#4c1"/>'
    svgCode=svgCode+'<path fill="#4c1" d="M55 0h4v20h-4z"/>'
else:
    svgCode=svgCode+'<rect rx="3" x="55" width="35" height="20" fill="#ffb521"/>'
    svgCode=svgCode+'<path fill="#ffb521" d="M55 0h4v20h-4z"/>'
svgCode=svgCode+'<rect rx="3" width="90" height="20" fill="url(#a)"/>'
svgCode=svgCode+'<g fill="#fff" text-anchor="middle" font-family="DejaVu Sans,Verdana,Geneva,sans-serif" font-size="11">'
svgCode=svgCode+'    <text x="26.5" y="15" fill="#010101" fill-opacity=".3">coverage</text>'
svgCode=svgCode+'    <text x="26.5" y="14">coverage</text>'
svgCode=svgCode+'    <text x="72.5" y="15" fill="#010101" fill-opacity=".3">'+coverage_percentage+'</text>'
svgCode=svgCode+'    <text x="72.5" y="14">'+coverage_percentage+'</text>'
svgCode=svgCode+'</g>'
svgCode=svgCode+'</svg>'

outputSVGFile.write(svgCode)
outputSVGFile.close()

#Convert to png file
outputPNGFile = open(outputPNGFilepath, 'w')
cairosvg.svg2png(bytestring=svgCode,write_to=outputPNGFile)
outputSVGFile.close()
