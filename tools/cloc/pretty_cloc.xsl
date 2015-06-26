<?xml version="1.0" encoding="US-ASCII"?>
<!-- XLS file by Paul Schwann, January 2009.
     Fixes for by-file and by-file-by-lang by d_uragan, November 2010.
     Added % columns, reformat summary and css by Nicolas Fauvet, December 2011
     -->
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
  <xsl:output method="html"/>
  <xsl:template match="/">
    <html xmlns="http://www.w3.org/1999/xhtml">
      <head>
        <title>CLOC Results</title>
      </head>
      <style type="text/css">
          body {
            color: #151515;
            font-family: helvetica neue,tahoma,arial,sans-serif;
            font-size: 82%;
            line-height: 1.5em;
          }
          table, td, tr, th {
            border: thin solid #999999;
            border-spacing: 0;
          }
          table {
            border-collapse: collapse; 
          }
          th {
            border: 1px solid #CCCCCC;
            padding: 4px;
            background-color: #E3E9EF;
          }
          td {
            border: 1px solid #CCCCCC;
            padding: 4px;
            text-align: center;
          }
          .percent {
            font-size: 70%;
          }
      </style>
      <body>
        <h2>CLOC results</h2>
        <h3>
          <ul>
            <xsl:value-of select="results/header/n_files"/> files (<xsl:value-of select="results/header/n_lines"/> lines) processed in <xsl:value-of select="results/header/elapsed_seconds"/> second(s) (<xsl:value-of select="round(results/header/files_per_second)"/> files/s, <xsl:value-of select="round(results/header/lines_per_second)"/> lines/s)
          </ul>
        </h3>
        <ul>
          <table>
            <tr>
              <th>Language</th>
              <th>Files</th>
              <th>Blank</th>
              <th>%</th>
              <th>Comment</th>
              <th>%</th>
              <th>Code</th>
              <th>%</th>
              <th>Total</th>
            </tr>
            <xsl:for-each select="results/languages/language">
              <tr>
                <th>
                  <xsl:value-of select="@name"/>
                </th>
                <td>
                  <xsl:value-of select="@files_count"/>
                </td>
                <td>
                  <xsl:value-of select="@blank"/>
                </td>
                <td class="percent">
                  <xsl:value-of select="round(@blank div (@blank+@comment+@code) * 100)"/>
                </td>
                <td>
                  <xsl:value-of select="@comment"/>
                </td>
                <td class="percent">
                  <xsl:value-of select="round(@comment div (@blank+@comment+@code) * 100)"/>
                </td>
                <td>
                  <xsl:value-of select="@code"/>
                </td>
                <td class="percent">
                  <xsl:value-of select="round(@code div (@blank+@comment+@code) * 100)"/>
                </td>
                <td>
                  <xsl:value-of select="@blank+@comment+@code"/>
                </td>
              </tr>
            </xsl:for-each>
            <tr>
              <th>Total</th>
              <th>
                <xsl:value-of select="results/languages/total/@sum_files"/>
              </th>
              <th>
                <xsl:value-of select="results/languages/total/@blank"/>
              </th>
              <th class="percent">
                <xsl:value-of select="round(results/languages/total/@blank div (results/languages/total/@blank+results/languages/total/@comment+results/languages/total/@code) * 100)"/>
              </th>
              <th>
                <xsl:value-of select="results/languages/total/@comment"/>
              </th>
              <th class="percent">
                <xsl:value-of select="round(results/languages/total/@comment div (results/languages/total/@blank+results/languages/total/@comment+results/languages/total/@code) * 100)"/>
              </th>
              <th>
                <xsl:value-of select="results/languages/total/@code"/>
              </th>
              <th class="percent">
                <xsl:value-of select="round(results/languages/total/@code div (results/languages/total/@blank+results/languages/total/@comment+results/languages/total/@code) * 100)"/>
              </th>
              <th>
                <xsl:value-of select="results/languages/total/@blank+results/languages/total/@comment+results/languages/total/@code"/>
              </th>
            </tr>
          </table>
        </ul>
      </body>
    </html>
  </xsl:template>
</xsl:stylesheet>
