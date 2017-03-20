# $FreeBSD: releng/11.0/share/skel/dot.cshrc 278616 2015-02-12 05:35:00Z cperciva $
#
# .cshrc - csh resource script, read at beginning of execution by each shell
#
# see also csh(1), environ(7).
# more examples available at /usr/share/examples/csh/
#

alias h		history 25
alias j		jobs -l
alias ls        ls -G
alias la	ls -aF
alias lf	ls -FA
alias ll	ls -lAF
alias grep	grep --color=auto

alias remote_grimlock 'ssh -Cvp 741 f1y@grimlock'
alias remote_www 'ssh -Cvp 741 bsobel@grimlock-www'
alias remote_gateway 'ssh -Cvp 2220 bsobel@gateway'
alias remote_router 'ssh -Cvp 741 bsobel@router'

# These are normally set through /etc/login.conf.  You may override them here
# if wanted.
# set path = (/sbin /bin /usr/sbin /usr/bin /usr/local/sbin /usr/local/bin $HOME/bin)
# setenv	BLOCKSIZE	K
# A righteous umask
# umask 22

setenv	EDITOR	vim
setenv	PAGER	less

if ($?prompt) then
	# An interactive shell -- set some stuff up
	set prompt = "%N@%m:%~ %# "
	set promptchars = "%#"

	set filec
	set history = 1000
	set savehist = (1000 merge)
	set autolist = ambiguous
	# Use history to aid expansion
	set autoexpand
	set autorehash
	set mail = (/var/mail/$USER)
	if ( $?tcsh ) then
		bindkey "^W" backward-delete-word
		bindkey -k up history-search-backward
		bindkey -k down history-search-forward
	endif

endif

complete {man,which,where} 'p/1/c/'
complete radio.sh 'p/1/(antyradio off)/'
complete zdalny.sh 'p/1/(204 205 perun we)/'
