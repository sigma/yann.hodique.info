+++
title: "{{ replace .TranslationBaseName "-" " " }}.bib"
date: {{ .Date }}
categories = [ "bib" ]
+++

{{< highlight bib >}}
{{< rawfile file="/static/bib/{{ replace .TranslationBaseName "-" " " }}.bib" >}}
{{< /highlight >}}
