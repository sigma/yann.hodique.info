+++
title = "Html description in Dokuwiki feeds"
date = "2007-02-05T17:48:00-00:00"
categories = [ "blog" ]
tags = [ "dokuwiki" ]

+++


A quick hack for displaying complete posts (in html) in rss feeds.  I'm using
the blogging suite from Esther Brunner
([http://wikidesign.ch/](http://wikidesign.ch/)). The plugins are written in
such a way that they don't allow html in feeds.  I had to modify it that way :

In  `helper.php` (from blog plugin)

{{< highlight php >}}
...
// dump complete content
$desc = p_cached_output(wikiFN($id,$rev),'simplehtml',$id);

$result[$key] = array(
  'id'       => $id,
  'date'     => $cdate,
  'exists'   => true,
  'perm'     => $perm,
  'html'     => $desc,
);
{{< /highlight >}}

In  `feed.php` (from feed plugin)

{{< highlight php >}}
// description
if ($page['desc']) $item->description = htmlspecialchars($page['desc']);
elseif ($page['html']) $item->description = $page['html'];
else $item->description = htmlspecialchars($meta['description']['abstract']);
{{< /highlight >}}

Also, I introduced a "simplehtml" parser, which is a clone of "xhtml" except
that it wipes out the css styling.
