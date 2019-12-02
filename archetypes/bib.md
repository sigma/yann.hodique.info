+++
title = "{{ replace .TranslationBaseName "-" " " }}.bib"
date = {{ .Date }}
categories = [ "bib" ]
+++

{{< bibfile file="bib/{{ replace .TranslationBaseName "-" " " }}.bib" >}}
