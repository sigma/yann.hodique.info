+++
title = "BibTeX Plugin for Dokuwiki"
date = "2012-04-09T17:48:00-00:00"
categories = ["projects"]

+++

Since this site is also my professional webpage, I have a need for referencing
articles I have read or written (see [subject]({{< ref "/public/subject.md"
>}}) and [bibliography]({{< ref "/public/bibliography.md" >}}) for example).
A previous version of my site was using [PmWiki](http://www.pmwiki.org) and
I had developped with some friends of mine a BibTeX plugin
([BibtexRef](http://www.pmwiki.org/wiki/Cookbook/BibtexRef)) to fit our needs
and the ones of our research teams.  When I switched to Dokuwiki, a plugin
already existed
([bibtex](http://stat.genopole.cnrs.fr/~cambroise/doku.php?id=softwares:dokuwikibibtexplugin))
but it was not sufficient for what I expected.

My code is a modification of both projects that has enough features for what I need, and probably the potential to be extended if needed.

## Installation

*  unzip the plugin ([bibtex.zip](/media/projects/bibtex.zip)) in lib/plugins
*  install the 2 additional files given 

## Markup

There are two commands available for wiki pages:

*  `<BIBTEX [file=value_file] [style=value_style] [sort=value_sort]>` which displays all the citations contained in the page  `bib:value_file`, with  formatting style  `value_style` ordered by  `value_sort`
*  `{[file:reference]}` which allows to cite a reference extracted from a file

### Example 1

 `<BIBTEX: file=hodique>` parses the content of [hodique.bib]({{< ref "/bib/hodique.md" >}}) and outputs :

### Example 2

 `{[hodique:ghsr06ijpeds]}` looks for the reference ghsr06ijpeds in file [hodique.bib]({{< ref "/bib/hodique.md" >}}) and outputs :

## Storage

The data for building the citations and references are stored in one or more wiki pages that reflect .bib files.
For example, in this site there are 2 bibtex  "files" : [hodique.bib]({{< ref "/bib/hodique.md" >}}) for my publications and [biblio.bib]({{< ref "/bib/biblio.md" >}}) for related works by others.

Please note that the plugin will parse the content of the  **underlying file**, not the interpreted page, so that it is very important that no additional markup is present in these.

I find it convenient to prefix every single line of my bib pages with two spaces, so that the entire data is not interpreted by the wiki engine while remaining valid bibtex syntax : see the source of [hodique.bib]({{< ref "/bib/hodique.md" >}}) for example.

## Citation pages

When you cite a reference (or when you click the (BIB) link at the end of a citation), you have access to a dedicated page that summarizes the paper.
Of course this page is not a wiki page, it is generated on demand from the data in the bibtex files.

To obtain this, I had to add 2 files to my Dokuwiki installation :

*  a script to allow displaying such content (lib/exe/bib.php). It is called  *via* a rewrite rule in .htaccess :  `RewriteRule ^_bib/(.*)/(.*)           lib/exe/bib.php?file=$1&ref=$2  [QSA,L]`

```php
<?php

if(!defined('DOKU_INC')) define('DOKU_INC',realpath(dirname(__FILE__).'/../../').'/');
if(!defined('DOKU_PLUGIN')) define('DOKU_PLUGIN',DOKU_INC.'lib/plugins/');
require_once(DOKU_INC.'inc/init.php');
require_once(DOKU_INC.'inc/common.php');
require_once(DOKU_INC.'inc/html.php');
require_once(DOKU_INC.'inc/pageutils.php');
require_once(DOKU_INC.'inc/confutils.php');
require_once(DOKU_INC.'inc/auth.php');
require_once(DOKU_INC.'inc/template.php');
require_once(DOKU_PLUGIN.'bibtex/syntax/bibtex.php');

//close session
session_write_close();

function bib_content() {

    $pathToOsbib = DOKU_PLUGIN.'bibtex/OSBib/';
    include_once($pathToOsbib.'format/bibtexParse/PARSEENTRIES.php');
    include_once( $pathToOsbib.'format/BIBFORMAT.php');

    $data = parseBibFile($_REQUEST['file']);

    /* Get the bibtex entries into an associative array */
    $parse = NEW PARSEENTRIES();
    $parse->expandMacro = TRUE;
    $parse->fieldExtract = TRUE;
    $parse->removeDelimit = TRUE;
    $parse->loadBibtexString($data);
    $parse->extractEntries();
    list($preamble, $strings, $entries) = $parse->returnArrays();

    $ref = $_REQUEST['ref'];
    foreach ($entries as $entry) {
        if (trim($entry['bibtexCitation']) == $ref)
            return rawOutput($entry);
    }
}

include(template('bib.php'));

//Setup VIM: ex: et ts=2 enc=utf-8 :
?>
```

*  a theme file to render it (lib/tpl/artic/bib.php since I use Artic theme from [http://chimeric.de](http://chimeric.de)). You may adapt your own theme instead. The important thing is  `<?php print bib_content() ?>` where the page main content should be. Also I suppressed action links in those generated pages since they don't make sense.

```php
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN"
 "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<?php
/**
 * DokuWiki Arctic Template
 *
 * This is the template you need to change for the overall look
 * of DokuWiki.
 *
 * You should leave the doctype at the very top - It should
 * always be the very first line of a document.
 *
 * @link   http://wiki.splitbrain.org/wiki:tpl:templates
 * @author Andreas Gohr <andi@splitbrain.org>
 * additional editing by
 * @author Michael Klier <chi@chimeric.de>
 * @link   http://chimeric.de/wiki/dokuwiki/templates/arctic/
 *
 */

 require_once(dirname(__FILE__).'/tpl_functions.php');
 $sepchar = tpl_getConf('sepchar');
?>
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php echo $conf['lang']?>"
 lang="<?php echo $conf['lang']?>" dir="<?php echo $lang['direction']?>">
<head>
  <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
  <title>
    <?php tpl_pagetitle()?>
    [<?php echo strip_tags($conf['title'])?>]
  </title>

  <?php tpl_metaheaders()?>

  <link rel="shortcut icon" href="<?php echo DOKU_TPL?>images/favicon.ico" />

  <?php /*old includehook*/ @include(dirname(__FILE__).'/meta.html')?>
  <?php
    if (file_exists(DOKU_PLUGIN.'googleanalytics/code.php')) include_once(DOKU_PLUGIN.'googleanalytics/code.php');
    if (function_exists('ga_google_analytics_code')) ga_google_analytics_code();
  ?>
</head>

<body>
<?php /*old includehook*/ @include(dirname(__FILE__).'/topheader.html')?>
<div id="wrapper">
  <div class="dokuwiki">

    <?php html_msgarea()?>

    <div class="stylehead">
      <div class="header">
        <div class="pagename">
          [[<?php print $_REQUEST['file'] . ":" . $_REQUEST['ref'] ?>]]
        </div>
        <div class="logo">
          <?php tpl_link(wl(),$conf['title'],'name="dokuwiki__top" accesskey="h" title="[ALT+H]"')?>
        </div>
      </div>
    
      <?php if(tpl_getConf('breadcrumbs') == 'top' or tpl_getConf('breadcrumbs') == 'both') {?> 
      <div class="breadcrumbs">
        <?php ($conf['youarehere'] != 1) ? tpl_breadcrumbs() : tpl_youarehere();?>
      </div>
      <?php } ?>

      <?php /*old includehook*/ @include(dirname(__FILE__).'/header.html')?>
      </div>

      <div class="bar" id="bar__top">
        <div class="bar-left">
        </div>
        <div class="bar-right">

          <?php
            switch(tpl_getConf('wiki_actionlinks')) {
              case('buttons'):
                if(tpl_getConf('sidebar') == 'none') tpl_searchform();
                tpl_button('admin');
                tpl_button('profile');
                tpl_button('recent');
                tpl_button('index');
                tpl_button('login');
                break;
              case('links'):
                if(tpl_getConf('sidebar') == 'none') tpl_searchform();
                if(tpl_actionlink('admin')) print ($sepchar);
                if(tpl_actionlink('profile')) print ($sepchar);
                tpl_actionlink('recent');
                print ($sepchar);
                tpl_actionlink('index');
                print ($sepchar);
                tpl_actionlink('login');
                break;
            }
          ?>

        </div>
    </div>

    <?php flush()?>

    <?php /*old includehook*/ @include(dirname(__FILE__).'/pageheader.html')?>

    <?php if(tpl_getConf('sidebar') == 'left') { ?>

      <?php if($ACT != 'diff' && $ACT != 'edit' && $ACT != 'preview') { ?>

        <div class="left_sidebar">
          <?php tpl_searchform() ?>
          <?php tpl_sidebar() ?>
        </div>
        <div class="right_page">
          <!-- wikipage start -->
          <?php print bib_content() ?>
          <!-- wikipage stop -->
        </div>

      <?php } else { ?>

        <div class="page">
          <!-- wikipage start -->
          <?php print bib_content() ?>
          <!-- wikipage stop -->
        </div> 

      <?php } ?>

    <?php } elseif(tpl_getConf('sidebar') == 'right') { ?>
      <?php if($ACT != 'diff' && $ACT != 'edit' && $ACT != 'preview') { ?>

        <div class="left_page">
          <!-- wikipage start -->
          <?php print bib_content() ?>
          <!-- wikipage stop -->
        </div>
        <div class="right_sidebar">
          <?php tpl_searchform() ?>
          <?php tpl_sidebar() ?>
        </div>

      <?php } else { ?>

        <div class="page">
          <!-- wikipage start -->
          <?php print bib_content() ?>
          <!-- wikipage stop -->
        </div> 

      <?php }?>
    <?php } elseif(tpl_getConf('sidebar') == 'none') { ?>

      <div class="page">
      <!-- wikipage start -->
        <?php print bib_content() ?>
      <!-- wikipage stop -->
      </div>

    <?php } ?>
      <div class="stylefoot">
        <div class="meta">
          <div class="user">
          <?php tpl_userinfo()?>
          </div>
          <div class="doc">
          <?php tpl_pageinfo()?>
          </div>
        </div>
      </div>

    <div class="clearer"></div>

    <?php flush()?>

    <div class="bar" id="bar__bottom">
      <div class="bar-left">
      </div>
      <div class="bar-right">

        <?php 
          switch(tpl_getConf('wiki_actionlinks')) {
            case('buttons'):
              tpl_button('subscription');
              tpl_button('top');
              break;
            case('links'):
              if(tpl_actionlink('subscription')) print ($sepchar);
              tpl_actionlink('top');
              break;
          }
        ?>

      </div>
    </div>


  <?php /*old includehook*/ @include(dirname(__FILE__).'/footer.html')?>
  </div>
</div>

<div class="no"><?php tpl_indexerWebBug()?></div>
<?php //setup vim: ts=2 sw=2: ?>
</body>
</html>
```

## ToDo

This plugin is far from being complete. Here are the things that definitely should be done:

*  code cleanup. This is a hack, and not a really nice one
*  introduce configuration variables
*  allow more precise selection of papers to display (by author, by date, whatever)
*  stop depending on the storage backend as it could change
