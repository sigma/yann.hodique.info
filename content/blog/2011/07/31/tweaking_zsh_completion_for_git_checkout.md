+++
title = "Tweaking zsh completion for git checkout"
date = "2012-04-09T17:48:00-00:00"
categories = [ "blog" ]
tags = [ "zsh", "git" ]

+++

By default, zsh completion allows you to complete according to "git checkout"
semantics.  So `git checkout path/to/<TAB>` will actually work for updating
files from the index (and some other similar cases).

There's nothing to say about it, it's obviously correct. Now, there are
2 things that make it inconvenient for me:

*  first of all, I happen to work regularly with **huge** repositories, with
   **huge** working directories (approximately 35GB). Since the completion code
   ends up calling `git ls-files`, even caching doesn't help that much reducing
   the time spent building the completion table. And I wouldn't want to pay the
   price even the first time.
*  second, my branches generally use a directory-like layout (like
   `t/bugfix/<id>`, so that it makes the whole completion list
   rather confusing.

Due to the second point, I happen to always use `-` `-` to make it clear I'm
refering to filenames in the above scenario.  So instead of `git checkout
path/to/<TAB>`, I would actually always write `git checkout -` `-
path/to/<TAB>`.

From there, the solution to my problem is obvious, I just have to disable
filename completion until `-` `-` is seen.

Here's the code (put it as a `_git-checkout` function in your `$fpath`):

{{< highlight bash >}}
#compdef git-checkout
local curcontext=$curcontext state line
declare -A opt_args

local new_branch_reflog_arg

if (( words[(I)-b] > 0 )); then
    new_branch_reflog_arg='-l[create the new branch'\''s reflog]'
else
    new_branch_reflog_arg='-l[create the branch'\''s reflog]'
fi

if compset -N '--'; then
    __git_cached_files
else
    _arguments -C -S \
        '-q[suppress feedback messages]' \
                                                                                  \
        - switch-branch \
        '-f[force a complete re-read]' \
        '-b[create a new branch based at given branch]: :__git_guard_branch-name' \
        {-t,--track}'[set up configuration so pull merges from the start point]' \
        '--no-track[override the branch.autosetupmerge configuration variable]' \
        $new_branch_reflog_arg \
        '-m[3way merge current branch, working tree and new branch]' \
        '::branch:__git_revisions' \
                                                                                  \
        - update-files \
        '::tree-ish:__git_tree_ishs' && ret=0
fi
{{< /highlight >}}

You can compare to the original version in
`/usr/share/zsh/functions/Completion/Unix/_git`

Edit: thanks to Friedrich for pointing out the need for a `#compdef` cookie to
ensure proper behavior
