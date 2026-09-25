import{s as ao,x as r,y as a,b4 as lo,A as s,B as l,C as io,D as so,d as co,E as I,o as p,m as F,w as bo,b as i,p as $,n as b,J as go,Y as po,K as fo,$ as vo,q as P,r as C,bP as z,c as S,a as mo,b6 as ho,aV as uo,M,b7 as xo,ar as Co}from"./index-B6tVVyCI.js";var zo={paddingSmall:"12px 16px 12px",paddingMedium:"19px 24px 20px",paddingLarge:"23px 32px 24px",paddingHuge:"27px 40px 28px",titleFontSizeSmall:"16px",titleFontSizeMedium:"18px",titleFontSizeLarge:"18px",titleFontSizeHuge:"18px",closeIconSize:"18px",closeSize:"22px"};function So(t){const{primaryColor:y,borderRadius:m,lineHeight:e,fontSize:f,cardColor:g,textColor2:h,textColor1:k,dividerColor:c,fontWeightStrong:d,closeIconColor:o,closeIconColorHover:n,closeIconColorPressed:v,closeColorHover:u,closeColorPressed:w,modalColor:B,boxShadow1:E,popoverColor:_,actionColor:x}=t;return{...zo,lineHeight:e,color:g,colorModal:B,colorPopover:_,colorTarget:y,colorEmbedded:x,colorEmbeddedModal:x,colorEmbeddedPopover:x,textColor:h,titleTextColor:k,borderColor:c,actionColor:x,titleFontWeight:d,closeColorHover:u,closeColorPressed:w,closeBorderRadius:m,closeIconColor:o,closeIconColorHover:n,closeIconColorPressed:v,fontSizeSmall:f,fontSizeMedium:f,fontSizeLarge:f,fontSizeHuge:f,boxShadow:E,borderRadius:m}}const yo={name:"Card",common:ao,self:So},R=a("card-content",`
 flex: 1;
 min-width: 0;
 box-sizing: border-box;
 padding: 0 var(--n-padding-left) var(--n-padding-bottom) var(--n-padding-left);
 font-size: var(--n-font-size);
`);var ko=r([a("card",`
 font-size: var(--n-font-size);
 line-height: var(--n-line-height);
 display: flex;
 flex-direction: column;
 width: 100%;
 box-sizing: border-box;
 position: relative;
 border-radius: var(--n-border-radius);
 background-color: var(--n-color);
 color: var(--n-text-color);
 word-break: break-word;
 transition: 
 color .3s var(--n-bezier),
 background-color .3s var(--n-bezier),
 box-shadow .3s var(--n-bezier),
 border-color .3s var(--n-bezier);
 `,[lo({background:"var(--n-color-modal)"}),s("hoverable",[r("&:hover","box-shadow: var(--n-box-shadow);")]),s("content-segmented",[r(">",[a("card-content",`
 padding-top: var(--n-padding-bottom);
 `),l("content-scrollbar",[r(">",[a("scrollbar-container",[r(">",[a("card-content",`
 padding-top: var(--n-padding-bottom);
 `)])])])])])]),s("content-soft-segmented",[r(">",[a("card-content",`
 margin: 0 var(--n-padding-left);
 padding: var(--n-padding-bottom) 0;
 `),l("content-scrollbar",[r(">",[a("scrollbar-container",[r(">",[a("card-content",`
 margin: 0 var(--n-padding-left);
 padding: var(--n-padding-bottom) 0;
 `)])])])])])]),s("footer-segmented",[r(">",[l("footer",`
 padding-top: var(--n-padding-bottom);
 `)])]),s("footer-soft-segmented",[r(">",[l("footer",`
 padding: var(--n-padding-bottom) 0;
 margin: 0 var(--n-padding-left);
 `)])]),r(">",[a("card-header",`
 box-sizing: border-box;
 display: flex;
 align-items: center;
 font-size: var(--n-title-font-size);
 padding:
 var(--n-padding-top)
 var(--n-padding-left)
 var(--n-padding-bottom)
 var(--n-padding-left);
 `,[l("main",`
 font-weight: var(--n-title-font-weight);
 transition: color .3s var(--n-bezier);
 flex: 1;
 min-width: 0;
 color: var(--n-title-text-color);
 `),l("extra",`
 display: flex;
 align-items: center;
 font-size: var(--n-font-size);
 font-weight: 400;
 transition: color .3s var(--n-bezier);
 color: var(--n-text-color);
 `),l("close",`
 margin: 0 0 0 8px;
 transition:
 background-color .3s var(--n-bezier),
 color .3s var(--n-bezier);
 `)]),l("action",`
 box-sizing: border-box;
 transition:
 background-color .3s var(--n-bezier),
 border-color .3s var(--n-bezier);
 background-clip: padding-box;
 background-color: var(--n-action-color);
 `),R,a("card-content",[r("&:first-child",`
 padding-top: var(--n-padding-bottom);
 `)]),l("content-scrollbar",`
 display: flex;
 flex-direction: column;
 `,[r(">",[a("scrollbar-container",[r(">",[R])])]),r("&:first-child >",[a("scrollbar-container",[r(">",[a("card-content",`
 padding-top: var(--n-padding-bottom);
 `)])])])]),l("footer",`
 box-sizing: border-box;
 padding: 0 var(--n-padding-left) var(--n-padding-bottom) var(--n-padding-left);
 font-size: var(--n-font-size);
 `,[r("&:first-child",`
 padding-top: var(--n-padding-bottom);
 `)]),l("action",`
 background-color: var(--n-action-color);
 padding: var(--n-padding-bottom) var(--n-padding-left);
 border-bottom-left-radius: var(--n-border-radius);
 border-bottom-right-radius: var(--n-border-radius);
 `)]),a("card-cover",`
 overflow: hidden;
 width: 100%;
 border-radius: var(--n-border-radius) var(--n-border-radius) 0 0;
 `,[r("img",`
 display: block;
 width: 100%;
 `)]),s("bordered",`
 border: 1px solid var(--n-border-color);
 `,[r("&:target","border-color: var(--n-color-target);")]),s("action-segmented",[r(">",[l("action",[r("&:not(:first-child)",`
 border-top: 1px solid var(--n-border-color);
 `)])])]),s("content-segmented, content-soft-segmented",[r(">",[a("card-content",`
 transition: border-color 0.3s var(--n-bezier);
 `,[r("&:not(:first-child)",`
 border-top: 1px solid var(--n-border-color);
 `)]),l("content-scrollbar",`
 transition: border-color 0.3s var(--n-bezier);
 `,[r("&:not(:first-child)",`
 border-top: 1px solid var(--n-border-color);
 `)])])]),s("footer-segmented, footer-soft-segmented",[r(">",[l("footer",`
 transition: border-color 0.3s var(--n-bezier);
 `,[r("&:not(:first-child)",`
 border-top: 1px solid var(--n-border-color);
 `)])])]),s("embedded",`
 background-color: var(--n-color-embedded);
 `)]),io(a("card",`
 background: var(--n-color-modal);
 `,[s("embedded",`
 background-color: var(--n-color-embedded-modal);
 `)])),so(a("card",`
 background: var(--n-color-popover);
 `,[s("embedded",`
 background-color: var(--n-color-embedded-popover);
 `)]))]);const T={title:[String,Function],contentClass:String,contentStyle:[Object,String],contentScrollable:Boolean,headerClass:String,headerStyle:[Object,String],headerExtraClass:String,headerExtraStyle:[Object,String],footerClass:String,footerStyle:[Object,String],embedded:Boolean,segmented:{type:[Boolean,Object],default:!1},size:String,bordered:{type:Boolean,default:!0},closable:Boolean,hoverable:Boolean,role:String,onClose:[Function,Array],tag:{type:String,default:"div"},cover:Function,content:[String,Function],footer:Function,action:Function,headerExtra:Function,closeFocusable:Boolean},Bo=Co(T),$o={...I.props,...T};var Eo=co({name:"Card",props:$o,slots:Object,setup(t){const y=()=>{const{onClose:n}=t;n&&vo(n)},{inlineThemeDisabled:m,mergedClsPrefixRef:e,mergedRtlRef:f,mergedComponentPropsRef:g}=go(t),h=I("Card","-card",ko,yo,t,e),k=po("Card",f,e),c=P(()=>{var n,v;return t.size||((v=(n=g==null?void 0:g.value)==null?void 0:n.Card)==null?void 0:v.size)||"medium"}),d=P(()=>{const n=c.value,{self:{color:v,colorModal:u,colorTarget:w,textColor:B,titleTextColor:E,titleFontWeight:_,borderColor:x,actionColor:O,borderRadius:V,lineHeight:j,closeIconColor:H,closeIconColorHover:L,closeIconColorPressed:W,closeColorHover:K,closeColorPressed:N,closeBorderRadius:A,closeIconSize:D,closeSize:q,boxShadow:J,colorPopover:Y,colorEmbedded:G,colorEmbeddedModal:Q,colorEmbeddedPopover:U,[M("padding",n)]:X,[M("fontSize",n)]:Z,[M("titleFontSize",n)]:oo},common:{cubicBezierEaseInOut:eo}}=h.value,{top:ro,left:to,bottom:no}=xo(X);return{"--n-bezier":eo,"--n-border-radius":V,"--n-color":v,"--n-color-modal":u,"--n-color-popover":Y,"--n-color-embedded":G,"--n-color-embedded-modal":Q,"--n-color-embedded-popover":U,"--n-color-target":w,"--n-text-color":B,"--n-line-height":j,"--n-action-color":O,"--n-title-text-color":E,"--n-title-font-weight":_,"--n-close-icon-color":H,"--n-close-icon-color-hover":L,"--n-close-icon-color-pressed":W,"--n-close-color-hover":K,"--n-close-color-pressed":N,"--n-border-color":x,"--n-box-shadow":J,"--n-padding-top":ro,"--n-padding-bottom":no,"--n-padding-left":to,"--n-font-size":Z,"--n-title-font-size":oo,"--n-close-size":q,"--n-close-icon-size":D,"--n-close-border-radius":A}}),o=m?fo("card",P(()=>c.value[0]),d,t):void 0;return{rtlEnabled:k,mergedClsPrefix:e,mergedTheme:h,handleCloseClick:y,cssVars:m?void 0:d,themeClass:o==null?void 0:o.themeClass,onRender:o==null?void 0:o.onRender}},render(){const{segmented:t,bordered:y,hoverable:m,mergedClsPrefix:e,rtlEnabled:f,onRender:g,embedded:h,tag:k,$slots:c}=this;return g==null||g(),p(),F(k,{class:b([`${e}-card`,this.themeClass,h&&`${e}-card--embedded`,{[`${e}-card--rtl`]:f,[`${e}-card--content-scrollable`]:this.contentScrollable,[`${e}-card--content${typeof t!="boolean"&&t.content==="soft"?"-soft":""}-segmented`]:t===!0||t!==!1&&t.content,[`${e}-card--footer${typeof t!="boolean"&&t.footer==="soft"?"-soft":""}-segmented`]:t===!0||t!==!1&&t.footer,[`${e}-card--action-segmented`]:t===!0||t!==!1&&t.action,[`${e}-card--bordered`]:y,[`${e}-card--hoverable`]:m}]),style:$(this.cssVars),role:this.role},{default:bo(()=>[i(()=>C(c.cover,d=>{const o=this.cover?z([this.cover()]):d;return o&&(p(),S("div",{class:b(`${e}-card-cover`),role:"none"},[i(()=>o)],2))})),i(()=>C(c.header,d=>{const{title:o}=this,n=o?z(typeof o=="function"?[o()]:[o]):d;return n||this.closable?(p(),S("div",{key:1,class:b([`${e}-card-header`,this.headerClass]),style:$(this.headerStyle),role:"heading"},[mo("div",{class:b(`${e}-card-header__main`),role:"heading"},[i(()=>n)],2),i(()=>C(c["header-extra"],v=>{const u=this.headerExtra?z([this.headerExtra()]):v;return u&&(p(),S("div",{class:b([`${e}-card-header__extra`,this.headerExtraClass]),style:$(this.headerExtraStyle)},[i(()=>u)],6))})),i(()=>this.closable&&(p(),F(ho,{clsPrefix:e,class:b(`${e}-card-header__close`),onClick:this.handleCloseClick,focusable:this.closeFocusable,absolute:!0},null,8,["clsPrefix","class","onClick","focusable"])))],6)):null})),i(()=>C(c.default,d=>{const{content:o}=this,n=o?z(typeof o=="function"?[o()]:[o]):d;return n?this.contentScrollable?(p(),F(uo,{key:2,class:b(`${e}-card__content-scrollbar`),contentClass:[`${e}-card-content`,this.contentClass],contentStyle:this.contentStyle},{default:()=>n},1032,["class","contentClass","contentStyle"])):(p(),S("div",{key:3,class:b([`${e}-card-content`,this.contentClass]),style:$(this.contentStyle),role:"none"},[i(()=>n)],6)):null})),i(()=>C(c.footer,d=>{const o=this.footer?z([this.footer()]):d;return o&&(p(),S("div",{class:b([`${e}-card__footer`,this.footerClass]),style:$(this.footerStyle),role:"none"},[i(()=>o)],6))})),i(()=>C(c.action,d=>{const o=this.action?z([this.action()]):d;return o&&(p(),S("div",{class:b(`${e}-card__action`),role:"none"},[i(()=>o)],2))}))]),_:2},1032,["class","style","role"])}});export{Eo as C,T as a,Bo as b,yo as c};
