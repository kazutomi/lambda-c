/*
 * debug.h - debug facilities
 */
#if !defined(_DEBUG_H_INCLUDED_)

enum level {
  L_MESG = 0,	/* always print */
  /* the latter, the more verbose; cf. syslog(3) */
  L_CRIT,
  L_ERR,
  L_WARNING,
  L_NOTICE,
  L_INFO,
  L_CURDEBUG,
  L_DEBUG,
  /* end of levels */
  L_PRODUCT = L_ERR,	/* product level */
};

enum facility {
  F_MAIN = 0,
  F_POOL,
  F_PARSER,
  F_LAMBOPS,
  F_STRLEXP,
  F_INTERFACE,
  F_MISC,
  F_NUMFACIL,	/* number of debug'ged facilities */
};

/*
 * for convenience for calling msg_generic()
 * 'debugfacility' must be defined, probably as static variables.
 *
 * XXX: C99 required
 */
#define msg_crit(...)     msg_generic(stderr, debugfacility, L_CRIT,     __VA_ARGS__)
#define msg_err(...)      msg_generic(stderr, debugfacility, L_ERR,      __VA_ARGS__)
#define msg_warning(...)  msg_generic(stderr, debugfacility, L_WARNING,  __VA_ARGS__)
#define msg_notice(...)   msg_generic(stderr, debugfacility, L_NOTICE,   __VA_ARGS__)
#define msg_info(...)     msg_generic(stderr, debugfacility, L_INFO,     __VA_ARGS__)
#define msg_curdebug(...) msg_generic(stderr, debugfacility, L_CURDEBUG, __VA_ARGS__)
#define msg_debug(...)    msg_generic(stderr, debugfacility, L_DEBUG,    __VA_ARGS__)
#define message(...)      msg_generic(stderr, debugfacility, L_MESG,     __VA_ARGS__)

/* debug level condition */
#define deblev(l) (dlevel[debugfacility]>=(l))

#define _DEBUG_H_INCLUDED_
#endif
/* EOF */
