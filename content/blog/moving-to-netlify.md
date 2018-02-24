+++
categories = ["blog"]
date = "2018-02-24T10:35:00-08:00"
tags = ["hugo", "netlify", "git"]
title = "Moving to Netlify"
+++

Today I moved this site from GitHub Pages to [Netlify](https://app.netlify.com/).
So far it looks pretty convincing, as it allowed for the following changes:

- simplified the deployment story, removing werckr from the equation. Don't get me wrong
  I like the service, it's just that the amount of time spent on it is excessive.
- the live-preview on pull requests will no doubt help me validate intrusive changes without
  breaking everything in the process (not that anybody would actually notice, but still :)).
  Additionally, that will enable me to hack on the site directly on GitHub, without the need
  for a local [hugo](https://gohugo.io) development environment.
- automatic [Let's Encrypt](https://letsencrypt.org/) certificates is a very nice plus,
  the lack of SSL termination with custom domains was my main grievance with GitHub Pages.
  
Other features I might use in the future:

- branch deployments sound cool for those occasional massive rewrites. I need to figure out the
  DNS implications though.
