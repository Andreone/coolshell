<?xml version="1.0"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  <xsl:output method="html"/>

  <xsl:template match="/">
    <html>
      <head>
        <style type="text/css">
          body {
            font-family: helvetica neue,tahoma,arial,sans-serif;
            font-size: 82%;
            line-height: 1.5em;
          }
          table {
            border-collapse: collapse;
            border-spacing: 0;
          }
          th {
            border: 1px solid #CCCCCC;
            padding: 4px;
            background-color: #BBA2FF;
          }
          td {
            border: 1px solid #CCCCCC;
            padding: 4px;
          }

          .color_error {
            background-color: #FFBBBB;
          }
          .color_warning {
            background-color: #F1FFB8;
          }
          .color_performance {
            background-color:#F4E8FF;
          }
          .color_portability {
            background-color: #CDE9FF;
          }
          .color_style {
            background-color: #C5FFB8;
          }
        </style>
      </head>
      <body>
        <h1>Cppcheck report</h1>

        <h2>Summary</h2>
        <h3>
          <xsl:value-of select="count(//error[@id!='missingInclude'])" /> issue(s) found</h3>
        <table>
          <tr>
            <th>Severity</th>
            <th>Count</th>
          </tr>
          <tr class="color_error">
            <td>error</td>
            <td align="right">
              <xsl:value-of select="count(//error[@severity='error'])" />
            </td>
          </tr>
          <tr class="color_warning">
            <td>warning</td>
            <td align="right">
              <xsl:value-of select="count(//error[@severity='warning'])" />
            </td>
          </tr>
          <tr class="color_style">
            <td>style</td>
            <td align="right">
              <xsl:value-of select="count(//error[@severity='style'])-count(//error[@id='missingInclude'])" />
            </td>
          </tr>
          <tr class="color_performance">
            <td>performance</td>
            <td align="right">
              <xsl:value-of select="count(//error[@severity='performance'])" />
            </td>
          </tr>
          <tr class="color_portability">
            <td>portability</td>
            <td align="right">
              <xsl:value-of select="count(//error[@severity='portability'])" />
            </td>
          </tr>
        </table>

        <h2>Details</h2>
        <table>
          <tr>
            <th>File</th>
            <th>Type</th>
            <th>Severity</th>
            <th>Message</th>
          </tr>
          <xsl:apply-templates select="results/error[@id!='missingInclude']">
            <xsl:sort select="@file"/>
          </xsl:apply-templates>
        </table>
      </body>
    </html>
  </xsl:template>

  <xsl:template match="error">
    <xsl:variable name="severity" select="@severity" />

    <tr class="color_{$severity}">
      <td>
        <xsl:value-of select="@file" /> (<xsl:value-of select="@line"/>)
      </td>
      <td>
        <xsl:value-of select="@severity" />
      </td>
      <td>
        <xsl:value-of select="@id" />
      </td>
      <td>
        <xsl:value-of select="@msg" />
      </td>
    </tr>
  </xsl:template>

</xsl:stylesheet>
