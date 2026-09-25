import{ab as Co,aC as yo,Z as go,q as L,_ as Go,bb as to,x as P,bQ as Ro,y as ro,d as mo,o as F,c as _,n as V,aS as Wo,ah as Mo,j as Y,a4 as _o,s as Oo,v as Po,A as I,B as f,z as bo,aF as Ko,E as So,r as xo,b as W,m as ao,w as jo,f as fo,a2 as Vo,p as U,bO as Lo,aG as Qo,aZ as No,as as qo,F as ho,aa as Ao,J as Yo,Y as Xo,K as Zo,$ as Jo,M as t,aE as oo,bR as po}from"./index-B6tVVyCI.js";const vo=go("n-form-item");function Uo(e,{defaultSize:h="medium",mergedSize:p,mergedDisabled:u}={}){const n=Co(vo,null);Go(vo,null);const G=L(p?()=>p(n):()=>{const{size:S}=e;if(S)return S;if(n){const{mergedSize:R}=n;if(R.value!==void 0)return R.value}return h}),Q=L(u?()=>u(n):()=>{const{disabled:S}=e;return S!==void 0?S:n?n.disabled.value:!1}),O=L(()=>{const{status:S}=e;return S||(n==null?void 0:n.mergedValidationStatus.value)});return yo(()=>{n&&n.restoreValidation()}),{mergedSizeRef:G,mergedDisabledRef:Q,mergedStatusRef:O,nTriggerFormBlur(){n&&n.handleContentBlur()},nTriggerFormChange(){n&&n.handleContentChange()},nTriggerFormFocus(){n&&n.handleContentFocus()},nTriggerFormInput(){n&&n.handleContentInput()}}}const oe=to&&"chrome"in window;to&&navigator.userAgent.includes("Firefox");const ee=to&&navigator.userAgent.includes("Safari")&&!oe,{cubicBezierEaseInOut:M}=Ro;function re({duration:e=".2s",delay:h=".1s"}={}){return[P("&.fade-in-width-expand-transition-leave-from, &.fade-in-width-expand-transition-enter-to",{opacity:1}),P("&.fade-in-width-expand-transition-leave-to, &.fade-in-width-expand-transition-enter-from",`
 opacity: 0!important;
 margin-left: 0!important;
 margin-right: 0!important;
 `),P("&.fade-in-width-expand-transition-leave-active",`
 overflow: hidden;
 transition:
 opacity ${e} ${M},
 max-width ${e} ${M} ${h},
 margin-left ${e} ${M} ${h},
 margin-right ${e} ${M} ${h};
 `),P("&.fade-in-width-expand-transition-enter-active",`
 overflow: hidden;
 transition:
 opacity ${e} ${M} ${h},
 max-width ${e} ${M},
 margin-left ${e} ${M},
 margin-right ${e} ${M};
 `)]}var te=ro("base-wave",`
 position: absolute;
 left: 0;
 right: 0;
 top: 0;
 bottom: 0;
 border-radius: inherit;
`),ne=mo({name:"BaseWave",props:{clsPrefix:{type:String,required:!0}},setup(e){Wo("-base-wave",te,_o(e,"clsPrefix"));const h=Y(null),p=Y(!1);let u=null;return yo(()=>{u!==null&&window.clearTimeout(u)}),{active:p,selfRef:h,play(){u!==null&&(window.clearTimeout(u),p.value=!1,u=null),Mo(()=>{var n;(n=h.value)==null||n.offsetHeight,p.value=!0,u=window.setTimeout(()=>{p.value=!1,u=null},1e3)})}}},render(){const{clsPrefix:e}=this;return F(),_("div",{ref:"selfRef","aria-hidden":!0,class:V([`${e}-base-wave`,this.active&&`${e}-base-wave--active`])},null,2)}}),se={paddingTiny:"0 6px",paddingSmall:"0 10px",paddingMedium:"0 14px",paddingLarge:"0 18px",paddingRoundTiny:"0 10px",paddingRoundSmall:"0 14px",paddingRoundMedium:"0 18px",paddingRoundLarge:"0 22px",iconMarginTiny:"6px",iconMarginSmall:"6px",iconMarginMedium:"6px",iconMarginLarge:"6px",iconSizeTiny:"14px",iconSizeSmall:"18px",iconSizeMedium:"18px",iconSizeLarge:"20px",rippleDuration:".6s"};function ie(e){const{heightTiny:h,heightSmall:p,heightMedium:u,heightLarge:n,borderRadius:G,fontSizeTiny:Q,fontSizeSmall:O,fontSizeMedium:S,fontSizeLarge:R,opacityDisabled:A,textColor2:v,textColor3:no,primaryColorHover:b,primaryColorPressed:B,borderColor:X,primaryColor:w,baseColor:i,infoColor:E,infoColorHover:z,infoColorPressed:H,successColor:r,successColorHover:a,successColorPressed:o,warningColor:C,warningColorHover:y,warningColorPressed:$,errorColor:g,errorColorHover:x,errorColorPressed:D,fontWeight:k,buttonColor2:N,buttonColor2Hover:T,buttonColor2Pressed:l,fontWeightStrong:q}=e;return{...se,heightTiny:h,heightSmall:p,heightMedium:u,heightLarge:n,borderRadiusTiny:G,borderRadiusSmall:G,borderRadiusMedium:G,borderRadiusLarge:G,fontSizeTiny:Q,fontSizeSmall:O,fontSizeMedium:S,fontSizeLarge:R,opacityDisabled:A,colorOpacitySecondary:"0.16",colorOpacitySecondaryHover:"0.22",colorOpacitySecondaryPressed:"0.28",colorSecondary:N,colorSecondaryHover:T,colorSecondaryPressed:l,colorTertiary:N,colorTertiaryHover:T,colorTertiaryPressed:l,colorQuaternary:"#0000",colorQuaternaryHover:T,colorQuaternaryPressed:l,color:"#0000",colorHover:"#0000",colorPressed:"#0000",colorFocus:"#0000",colorDisabled:"#0000",textColor:v,textColorTertiary:no,textColorHover:b,textColorPressed:B,textColorFocus:b,textColorDisabled:v,textColorText:v,textColorTextHover:b,textColorTextPressed:B,textColorTextFocus:b,textColorTextDisabled:v,textColorGhost:v,textColorGhostHover:b,textColorGhostPressed:B,textColorGhostFocus:b,textColorGhostDisabled:v,border:`1px solid ${X}`,borderHover:`1px solid ${b}`,borderPressed:`1px solid ${B}`,borderFocus:`1px solid ${b}`,borderDisabled:`1px solid ${X}`,rippleColor:w,colorPrimary:w,colorHoverPrimary:b,colorPressedPrimary:B,colorFocusPrimary:b,colorDisabledPrimary:w,textColorPrimary:i,textColorHoverPrimary:i,textColorPressedPrimary:i,textColorFocusPrimary:i,textColorDisabledPrimary:i,textColorTextPrimary:w,textColorTextHoverPrimary:b,textColorTextPressedPrimary:B,textColorTextFocusPrimary:b,textColorTextDisabledPrimary:v,textColorGhostPrimary:w,textColorGhostHoverPrimary:b,textColorGhostPressedPrimary:B,textColorGhostFocusPrimary:b,textColorGhostDisabledPrimary:w,borderPrimary:`1px solid ${w}`,borderHoverPrimary:`1px solid ${b}`,borderPressedPrimary:`1px solid ${B}`,borderFocusPrimary:`1px solid ${b}`,borderDisabledPrimary:`1px solid ${w}`,rippleColorPrimary:w,colorInfo:E,colorHoverInfo:z,colorPressedInfo:H,colorFocusInfo:z,colorDisabledInfo:E,textColorInfo:i,textColorHoverInfo:i,textColorPressedInfo:i,textColorFocusInfo:i,textColorDisabledInfo:i,textColorTextInfo:E,textColorTextHoverInfo:z,textColorTextPressedInfo:H,textColorTextFocusInfo:z,textColorTextDisabledInfo:v,textColorGhostInfo:E,textColorGhostHoverInfo:z,textColorGhostPressedInfo:H,textColorGhostFocusInfo:z,textColorGhostDisabledInfo:E,borderInfo:`1px solid ${E}`,borderHoverInfo:`1px solid ${z}`,borderPressedInfo:`1px solid ${H}`,borderFocusInfo:`1px solid ${z}`,borderDisabledInfo:`1px solid ${E}`,rippleColorInfo:E,colorSuccess:r,colorHoverSuccess:a,colorPressedSuccess:o,colorFocusSuccess:a,colorDisabledSuccess:r,textColorSuccess:i,textColorHoverSuccess:i,textColorPressedSuccess:i,textColorFocusSuccess:i,textColorDisabledSuccess:i,textColorTextSuccess:r,textColorTextHoverSuccess:a,textColorTextPressedSuccess:o,textColorTextFocusSuccess:a,textColorTextDisabledSuccess:v,textColorGhostSuccess:r,textColorGhostHoverSuccess:a,textColorGhostPressedSuccess:o,textColorGhostFocusSuccess:a,textColorGhostDisabledSuccess:r,borderSuccess:`1px solid ${r}`,borderHoverSuccess:`1px solid ${a}`,borderPressedSuccess:`1px solid ${o}`,borderFocusSuccess:`1px solid ${a}`,borderDisabledSuccess:`1px solid ${r}`,rippleColorSuccess:r,colorWarning:C,colorHoverWarning:y,colorPressedWarning:$,colorFocusWarning:y,colorDisabledWarning:C,textColorWarning:i,textColorHoverWarning:i,textColorPressedWarning:i,textColorFocusWarning:i,textColorDisabledWarning:i,textColorTextWarning:C,textColorTextHoverWarning:y,textColorTextPressedWarning:$,textColorTextFocusWarning:y,textColorTextDisabledWarning:v,textColorGhostWarning:C,textColorGhostHoverWarning:y,textColorGhostPressedWarning:$,textColorGhostFocusWarning:y,textColorGhostDisabledWarning:C,borderWarning:`1px solid ${C}`,borderHoverWarning:`1px solid ${y}`,borderPressedWarning:`1px solid ${$}`,borderFocusWarning:`1px solid ${y}`,borderDisabledWarning:`1px solid ${C}`,rippleColorWarning:C,colorError:g,colorHoverError:x,colorPressedError:D,colorFocusError:x,colorDisabledError:g,textColorError:i,textColorHoverError:i,textColorPressedError:i,textColorFocusError:i,textColorDisabledError:i,textColorTextError:g,textColorTextHoverError:x,textColorTextPressedError:D,textColorTextFocusError:x,textColorTextDisabledError:v,textColorGhostError:g,textColorGhostHoverError:x,textColorGhostPressedError:D,textColorGhostFocusError:x,textColorGhostDisabledError:g,borderError:`1px solid ${g}`,borderHoverError:`1px solid ${x}`,borderPressedError:`1px solid ${D}`,borderFocusError:`1px solid ${x}`,borderDisabledError:`1px solid ${g}`,rippleColorError:g,waveOpacity:"0.6",fontWeight:k,fontWeightStrong:q}}const le={name:"Button",common:Oo,self:ie};function j(e){return Po(e,[255,255,255,.16])}function eo(e){return Po(e,[0,0,0,.12])}const ae=go("n-button-group");var de=P([ro("button",`
 margin: 0;
 font-weight: var(--n-font-weight);
 line-height: 1;
 font-family: inherit;
 padding: var(--n-padding);
 height: var(--n-height);
 font-size: var(--n-font-size);
 border-radius: var(--n-border-radius);
 color: var(--n-text-color);
 background-color: var(--n-color);
 width: var(--n-width);
 white-space: nowrap;
 outline: none;
 position: relative;
 z-index: auto;
 border: none;
 display: inline-flex;
 flex-wrap: nowrap;
 flex-shrink: 0;
 align-items: center;
 justify-content: center;
 user-select: none;
 -webkit-user-select: none;
 text-align: center;
 cursor: pointer;
 text-decoration: none;
 transition:
 color .3s var(--n-bezier),
 background-color .3s var(--n-bezier),
 opacity .3s var(--n-bezier),
 border-color .3s var(--n-bezier);
 `,[I("color",[f("border",{borderColor:"var(--n-border-color)"}),I("disabled",[f("border",{borderColor:"var(--n-border-color-disabled)"})]),bo("disabled",[P("&:focus",[f("state-border",{borderColor:"var(--n-border-color-focus)"})]),P("&:hover",[f("state-border",{borderColor:"var(--n-border-color-hover)"})]),P("&:active",[f("state-border",{borderColor:"var(--n-border-color-pressed)"})]),I("pressed",[f("state-border",{borderColor:"var(--n-border-color-pressed)"})])])]),I("disabled",{backgroundColor:"var(--n-color-disabled)",color:"var(--n-text-color-disabled)"},[f("border",{border:"var(--n-border-disabled)"})]),bo("disabled",[P("&:focus",{backgroundColor:"var(--n-color-focus)",color:"var(--n-text-color-focus)"},[f("state-border",{border:"var(--n-border-focus)"})]),P("&:hover",{backgroundColor:"var(--n-color-hover)",color:"var(--n-text-color-hover)"},[f("state-border",{border:"var(--n-border-hover)"})]),P("&:active",{backgroundColor:"var(--n-color-pressed)",color:"var(--n-text-color-pressed)"},[f("state-border",{border:"var(--n-border-pressed)"})]),I("pressed",{backgroundColor:"var(--n-color-pressed)",color:"var(--n-text-color-pressed)"},[f("state-border",{border:"var(--n-border-pressed)"})])]),I("loading","cursor: wait;"),ro("base-wave",`
 pointer-events: none;
 top: 0;
 right: 0;
 bottom: 0;
 left: 0;
 animation-iteration-count: 1;
 animation-duration: var(--n-ripple-duration);
 animation-timing-function: var(--n-bezier-ease-out), var(--n-bezier-ease-out);
 `,[I("active",{zIndex:1,animationName:"button-wave-spread, button-wave-opacity"})]),to&&"MozBoxSizing"in document.createElement("div").style?P("&::moz-focus-inner",{border:0}):null,f("border, state-border",`
 position: absolute;
 left: 0;
 top: 0;
 right: 0;
 bottom: 0;
 border-radius: inherit;
 transition: border-color .3s var(--n-bezier);
 pointer-events: none;
 `),f("border",`
 border: var(--n-border);
 `),f("state-border",`
 border: var(--n-border);
 border-color: #0000;
 z-index: 1;
 `),f("icon",`
 margin: var(--n-icon-margin);
 margin-left: 0;
 height: var(--n-icon-size);
 width: var(--n-icon-size);
 max-width: var(--n-icon-size);
 font-size: var(--n-icon-size);
 position: relative;
 flex-shrink: 0;
 `,[ro("icon-slot",`
 height: var(--n-icon-size);
 width: var(--n-icon-size);
 position: absolute;
 left: 0;
 top: 50%;
 transform: translateY(-50%);
 display: flex;
 align-items: center;
 justify-content: center;
 `,[Ko({top:"50%",originalTransform:"translateY(-50%)"})]),re()]),f("content",`
 display: flex;
 align-items: center;
 flex-wrap: nowrap;
 min-width: 0;
 `,[P("~",[f("icon",{margin:"var(--n-icon-margin)",marginRight:0})])]),I("block",`
 display: flex;
 width: 100%;
 `),I("dashed",[f("border, state-border",{borderStyle:"dashed !important"})]),I("disabled",{cursor:"not-allowed",opacity:"var(--n-opacity-disabled)"})]),P("@keyframes button-wave-spread",{from:{boxShadow:"0 0 0.5px 0 var(--n-ripple-color)"},to:{boxShadow:"0 0 0.5px 4.5px var(--n-ripple-color)"}}),P("@keyframes button-wave-opacity",{from:{opacity:"var(--n-wave-opacity)"},to:{opacity:0}})]);const ce={...So.props,color:String,textColor:String,text:Boolean,block:Boolean,loading:Boolean,disabled:Boolean,circle:Boolean,size:String,ghost:Boolean,round:Boolean,secondary:Boolean,tertiary:Boolean,quaternary:Boolean,strong:Boolean,focusable:{type:Boolean,default:!0},keyboard:{type:Boolean,default:!0},tag:{type:String,default:"button"},type:{type:String,default:"default"},dashed:Boolean,renderIcon:Function,iconPlacement:{type:String,default:"left"},attrType:{type:String,default:"button"},bordered:{type:Boolean,default:!0},onClick:[Function,Array],nativeFocusBehavior:{type:Boolean,default:!ee},spinProps:Object},ue=mo({name:"Button",props:ce,slots:Object,setup(e){const h=Y(null),p=Y(null),u=Y(!1),n=Ao(()=>!e.quaternary&&!e.tertiary&&!e.secondary&&!e.text&&(!e.color||e.ghost||e.dashed)&&e.bordered),G=Co(ae,{}),{inlineThemeDisabled:Q,mergedClsPrefixRef:O,mergedRtlRef:S,mergedComponentPropsRef:R}=Yo(e),{mergedSizeRef:A}=Uo({},{defaultSize:"medium",mergedSize:r=>{var $,g;const{size:a}=e;if(a)return a;const{size:o}=G;if(o)return o;const{mergedSize:C}=r||{};if(C)return C.value;const y=(g=($=R==null?void 0:R.value)==null?void 0:$.Button)==null?void 0:g.size;return y||"medium"}}),v=L(()=>e.focusable&&!e.disabled),no=r=>{var a;v.value||r.preventDefault(),!e.nativeFocusBehavior&&(r.preventDefault(),!e.disabled&&v.value&&((a=h.value)==null||a.focus({preventScroll:!0})))},b=r=>{var a;if(!e.disabled&&!e.loading){const{onClick:o}=e;o&&Jo(o,r),e.text||(a=p.value)==null||a.play()}},B=r=>{switch(r.key){case"Enter":if(!e.keyboard)return;u.value=!1}},X=r=>{switch(r.key){case"Enter":if(!e.keyboard||e.loading){r.preventDefault();return}u.value=!0}},w=()=>{u.value=!1},i=So("Button","-button",de,le,e,O),E=Xo("Button",S,O),z=L(()=>{const{common:{cubicBezierEaseInOut:r,cubicBezierEaseOut:a},self:o}=i.value,{rippleDuration:C,opacityDisabled:y,fontWeight:$,fontWeightStrong:g}=o,x=A.value,{dashed:D,type:k,ghost:N,text:T,color:l,round:q,circle:so,textColor:K,secondary:$o,tertiary:co,quaternary:To,strong:wo}=e,zo={"--n-font-weight":wo?g:$};let d={"--n-color":"initial","--n-color-hover":"initial","--n-color-pressed":"initial","--n-color-focus":"initial","--n-color-disabled":"initial","--n-ripple-color":"initial","--n-text-color":"initial","--n-text-color-hover":"initial","--n-text-color-pressed":"initial","--n-text-color-focus":"initial","--n-text-color-disabled":"initial"};const Z=k==="tertiary",uo=k==="default",s=Z?"default":k;if(T){const c=K||l;d={"--n-color":"#0000","--n-color-hover":"#0000","--n-color-pressed":"#0000","--n-color-focus":"#0000","--n-color-disabled":"#0000","--n-ripple-color":"#0000","--n-text-color":c||o[t("textColorText",s)],"--n-text-color-hover":c?j(c):o[t("textColorTextHover",s)],"--n-text-color-pressed":c?eo(c):o[t("textColorTextPressed",s)],"--n-text-color-focus":c?j(c):o[t("textColorTextHover",s)],"--n-text-color-disabled":c||o[t("textColorTextDisabled",s)]}}else if(N||D){const c=K||l;d={"--n-color":"#0000","--n-color-hover":"#0000","--n-color-pressed":"#0000","--n-color-focus":"#0000","--n-color-disabled":"#0000","--n-ripple-color":l||o[t("rippleColor",s)],"--n-text-color":c||o[t("textColorGhost",s)],"--n-text-color-hover":c?j(c):o[t("textColorGhostHover",s)],"--n-text-color-pressed":c?eo(c):o[t("textColorGhostPressed",s)],"--n-text-color-focus":c?j(c):o[t("textColorGhostHover",s)],"--n-text-color-disabled":c||o[t("textColorGhostDisabled",s)]}}else if($o){const c=uo?o.textColor:Z?o.textColorTertiary:o[t("color",s)],m=l||c,J=k!=="default"&&k!=="tertiary";d={"--n-color":J?oo(m,{alpha:Number(o.colorOpacitySecondary)}):o.colorSecondary,"--n-color-hover":J?oo(m,{alpha:Number(o.colorOpacitySecondaryHover)}):o.colorSecondaryHover,"--n-color-pressed":J?oo(m,{alpha:Number(o.colorOpacitySecondaryPressed)}):o.colorSecondaryPressed,"--n-color-focus":J?oo(m,{alpha:Number(o.colorOpacitySecondaryHover)}):o.colorSecondaryHover,"--n-color-disabled":o.colorSecondary,"--n-ripple-color":"#0000","--n-text-color":m,"--n-text-color-hover":m,"--n-text-color-pressed":m,"--n-text-color-focus":m,"--n-text-color-disabled":m}}else if(co||To){const c=uo?o.textColor:Z?o.textColorTertiary:o[t("color",s)],m=l||c;co?(d["--n-color"]=o.colorTertiary,d["--n-color-hover"]=o.colorTertiaryHover,d["--n-color-pressed"]=o.colorTertiaryPressed,d["--n-color-focus"]=o.colorSecondaryHover,d["--n-color-disabled"]=o.colorTertiary):(d["--n-color"]=o.colorQuaternary,d["--n-color-hover"]=o.colorQuaternaryHover,d["--n-color-pressed"]=o.colorQuaternaryPressed,d["--n-color-focus"]=o.colorQuaternaryHover,d["--n-color-disabled"]=o.colorQuaternary),d["--n-ripple-color"]="#0000",d["--n-text-color"]=m,d["--n-text-color-hover"]=m,d["--n-text-color-pressed"]=m,d["--n-text-color-focus"]=m,d["--n-text-color-disabled"]=m}else d={"--n-color":l||o[t("color",s)],"--n-color-hover":l?j(l):o[t("colorHover",s)],"--n-color-pressed":l?eo(l):o[t("colorPressed",s)],"--n-color-focus":l?j(l):o[t("colorFocus",s)],"--n-color-disabled":l||o[t("colorDisabled",s)],"--n-ripple-color":l||o[t("rippleColor",s)],"--n-text-color":K||(l?o.textColorPrimary:Z?o.textColorTertiary:o[t("textColor",s)]),"--n-text-color-hover":K||(l?o.textColorHoverPrimary:o[t("textColorHover",s)]),"--n-text-color-pressed":K||(l?o.textColorPressedPrimary:o[t("textColorPressed",s)]),"--n-text-color-focus":K||(l?o.textColorFocusPrimary:o[t("textColorFocus",s)]),"--n-text-color-disabled":K||(l?o.textColorDisabledPrimary:o[t("textColorDisabled",s)])};let io={"--n-border":"initial","--n-border-hover":"initial","--n-border-pressed":"initial","--n-border-focus":"initial","--n-border-disabled":"initial"};T?io={"--n-border":"none","--n-border-hover":"none","--n-border-pressed":"none","--n-border-focus":"none","--n-border-disabled":"none"}:io={"--n-border":o[t("border",s)],"--n-border-hover":o[t("borderHover",s)],"--n-border-pressed":o[t("borderPressed",s)],"--n-border-focus":o[t("borderFocus",s)],"--n-border-disabled":o[t("borderDisabled",s)]};const{[t("height",x)]:lo,[t("fontSize",x)]:Ho,[t("padding",x)]:Fo,[t("paddingRound",x)]:Bo,[t("iconSize",x)]:Eo,[t("borderRadius",x)]:Io,[t("iconMargin",x)]:Do,waveOpacity:ko}=o;return{"--n-bezier":r,"--n-bezier-ease-out":a,"--n-ripple-duration":C,"--n-opacity-disabled":y,"--n-wave-opacity":ko,...zo,...d,...io,...{"--n-width":so&&!T?lo:"initial","--n-height":T?"initial":lo,"--n-font-size":Ho,"--n-padding":so||T?"initial":q?Bo:Fo,"--n-icon-size":Eo,"--n-icon-margin":Do,"--n-border-radius":T?"initial":so||q?lo:Io}}}),H=Q?Zo("button",L(()=>{let r="";const{dashed:a,type:o,ghost:C,text:y,color:$,round:g,circle:x,textColor:D,secondary:k,tertiary:N,quaternary:T,strong:l}=e;a&&(r+="a"),C&&(r+="b"),y&&(r+="c"),g&&(r+="d"),x&&(r+="e"),k&&(r+="f"),N&&(r+="g"),T&&(r+="h"),l&&(r+="i"),$&&(r+=`j${po($)}`),D&&(r+=`k${po(D)}`);const{value:q}=A;return r+=`l${q[0]}`,r+=`m${o[0]}`,r}),z,e):void 0;return{selfElRef:h,waveElRef:p,mergedClsPrefix:O,mergedFocusable:v,mergedSize:A,showBorder:n,enterPressed:u,rtlEnabled:E,handleMousedown:no,handleKeydown:X,handleBlur:w,handleKeyup:B,handleClick:b,customColorCssVars:L(()=>{const{color:r}=e;if(!r)return null;const a=j(r);return{"--n-border-color":r,"--n-border-color-hover":a,"--n-border-color-pressed":eo(r),"--n-border-color-focus":a,"--n-border-color-disabled":r}}),cssVars:Q?void 0:z,themeClass:H==null?void 0:H.themeClass,onRender:H==null?void 0:H.onRender}},render(){const{mergedClsPrefix:e,tag:h,onRender:p}=this;p==null||p();const u=xo(this.$slots.default,n=>n&&(F(),_("span",{class:V(`${e}-button__content`)},[W(()=>n)],2)));return F(),ao(h,{ref:"selfElRef",class:V([this.themeClass,`${e}-button`,`${e}-button--${this.type}-type`,`${e}-button--${this.mergedSize}-type`,this.rtlEnabled&&`${e}-button--rtl`,this.disabled&&`${e}-button--disabled`,this.block&&`${e}-button--block`,this.enterPressed&&`${e}-button--pressed`,!this.text&&this.dashed&&`${e}-button--dashed`,this.color&&`${e}-button--color`,this.secondary&&`${e}-button--secondary`,this.loading&&`${e}-button--loading`,this.ghost&&`${e}-button--ghost`]),tabindex:this.mergedFocusable?0:-1,type:this.attrType,style:U(this.cssVars),disabled:this.disabled,onClick:this.handleClick,onBlur:this.handleBlur,onMousedown:this.handleMousedown,onKeyup:this.handleKeyup,onKeydown:this.handleKeydown},{default:jo(()=>[W(()=>this.iconPlacement==="right"&&u),fo(Vo,{width:!0},{default:()=>xo(this.$slots.icon,n=>(this.loading||this.renderIcon||n)&&(F(),_("span",{class:V(`${e}-button__icon`),style:U({margin:Lo(this.$slots.default)?"0":""})},[fo(Qo,null,{default:()=>this.loading?(F(),ao(No,qo({clsPrefix:e,key:"loading",class:`${e}-icon-slot`,strokeWidth:20},this.spinProps),null,16,["clsPrefix","class"])):(F(),_("div",{key:"icon",class:V(`${e}-icon-slot`),role:"none"},[this.renderIcon?(F(),_(ho,{key:0},[W(()=>this.renderIcon())],64)):(F(),_(ho,{key:1},[W(()=>n)],64))],2))},1024)],6)))},1024),W(()=>this.iconPlacement==="left"&&u),this.text?W(()=>null):(F(),ao(ne,{key:0,ref:"waveElRef",clsPrefix:e},null,8,["clsPrefix"])),this.showBorder?(F(),_("div",{key:2,"aria-hidden":!0,class:V(`${e}-button__border`),style:U(this.customColorCssVars)},null,6)):W(()=>null),this.showBorder?(F(),_("div",{key:4,"aria-hidden":!0,class:V(`${e}-button__state-border`),style:U(this.customColorCssVars)},null,6)):W(()=>null)]),_:2},1032,["class","tabindex","type","style","disabled","onClick","onBlur","onMousedown","onKeyup","onKeydown"])}}),fe=ue;export{ue as B,fe as X,le as b,ee as i,Uo as u};
