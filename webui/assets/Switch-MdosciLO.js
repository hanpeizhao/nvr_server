import{s as ve,aE as ge,y as I,B as i,aF as Y,x as j,A as s,z as q,d as me,E as Z,bO as A,o as b,c as C,a as _,b as l,r as k,n as r,m as G,p as J,J as pe,j as E,X as we,K as xe,q as K,$ as N,aZ as ye,as as ke,aG as Se,M as S,aX as O,aU as d,a4 as Be}from"./index-B6tVVyCI.js";import{u as Ce}from"./Button-BkAo_aWS.js";var _e={buttonHeightSmall:"14px",buttonHeightMedium:"18px",buttonHeightLarge:"22px",buttonWidthSmall:"14px",buttonWidthMedium:"18px",buttonWidthLarge:"22px",buttonWidthPressedSmall:"20px",buttonWidthPressedMedium:"24px",buttonWidthPressedLarge:"28px",railHeightSmall:"18px",railHeightMedium:"22px",railHeightLarge:"26px",railWidthSmall:"32px",railWidthMedium:"40px",railWidthLarge:"48px"};function $e(e){const{primaryColor:c,opacityDisabled:f,borderRadius:o,textColor3:v}=e;return{..._e,iconColor:v,textColor:"white",loadingColor:c,opacityDisabled:f,railColor:"rgba(0, 0, 0, .14)",railColorActive:c,buttonBoxShadow:"0 1px 4px 0 rgba(0, 0, 0, 0.3), inset 0 0 1px 0 rgba(0, 0, 0, 0.05)",buttonColor:"#FFF",railBorderRadiusSmall:o,railBorderRadiusMedium:o,railBorderRadiusLarge:o,buttonBorderRadiusSmall:o,buttonBorderRadiusMedium:o,buttonBorderRadiusLarge:o,boxShadowFocus:`0 0 0 2px ${ge(c,{alpha:.2})}`}}const ze={common:ve,self:$e};var Re=I("switch",`
 height: var(--n-height);
 min-width: var(--n-width);
 vertical-align: middle;
 user-select: none;
 -webkit-user-select: none;
 display: inline-flex;
 outline: none;
 justify-content: center;
 align-items: center;
`,[i("children-placeholder",`
 height: var(--n-rail-height);
 display: flex;
 flex-direction: column;
 overflow: hidden;
 pointer-events: none;
 visibility: hidden;
 `),i("rail-placeholder",`
 display: flex;
 flex-wrap: none;
 `),i("button-placeholder",`
 width: calc(1.75 * var(--n-rail-height));
 height: var(--n-rail-height);
 `),I("base-loading",`
 position: absolute;
 top: 50%;
 left: 50%;
 transform: translateX(-50%) translateY(-50%);
 font-size: calc(var(--n-button-width) - 4px);
 color: var(--n-loading-color);
 transition: color .3s var(--n-bezier);
 `,[Y({left:"50%",top:"50%",originalTransform:"translateX(-50%) translateY(-50%)"})]),i("checked, unchecked",`
 transition: color .3s var(--n-bezier);
 color: var(--n-text-color);
 box-sizing: border-box;
 position: absolute;
 white-space: nowrap;
 top: 0;
 bottom: 0;
 display: flex;
 align-items: center;
 line-height: 1;
 `),i("checked",`
 right: 0;
 padding-right: calc(1.25 * var(--n-rail-height) - var(--n-offset));
 `),i("unchecked",`
 left: 0;
 justify-content: flex-end;
 padding-left: calc(1.25 * var(--n-rail-height) - var(--n-offset));
 `),j("&:focus",[i("rail",`
 box-shadow: var(--n-box-shadow-focus);
 `)]),s("round",[i("rail","border-radius: calc(var(--n-rail-height) / 2);",[i("button","border-radius: calc(var(--n-button-height) / 2);")])]),q("disabled",[q("icon",[s("rubber-band",[s("pressed",[i("rail",[i("button","max-width: var(--n-button-width-pressed);")])]),i("rail",[j("&:active",[i("button","max-width: var(--n-button-width-pressed);")])]),s("active",[s("pressed",[i("rail",[i("button","left: calc(100% - var(--n-offset) - var(--n-button-width-pressed));")])]),i("rail",[j("&:active",[i("button","left: calc(100% - var(--n-offset) - var(--n-button-width-pressed));")])])])])])]),s("active",[i("rail",[i("button","left: calc(100% - var(--n-button-width) - var(--n-offset))")])]),i("rail",`
 overflow: hidden;
 height: var(--n-rail-height);
 min-width: var(--n-rail-width);
 border-radius: var(--n-rail-border-radius);
 cursor: pointer;
 position: relative;
 transition:
 opacity .3s var(--n-bezier),
 background .3s var(--n-bezier),
 box-shadow .3s var(--n-bezier);
 background-color: var(--n-rail-color);
 `,[i("button-icon",`
 color: var(--n-icon-color);
 transition: color .3s var(--n-bezier);
 font-size: calc(var(--n-button-height) - 4px);
 position: absolute;
 left: 0;
 right: 0;
 top: 0;
 bottom: 0;
 display: flex;
 justify-content: center;
 align-items: center;
 line-height: 1;
 `,[Y()]),i("button",`
 align-items: center; 
 top: var(--n-offset);
 left: var(--n-offset);
 height: var(--n-button-height);
 width: var(--n-button-width-pressed);
 max-width: var(--n-button-width);
 border-radius: var(--n-button-border-radius);
 background-color: var(--n-button-color);
 box-shadow: var(--n-button-box-shadow);
 box-sizing: border-box;
 cursor: inherit;
 content: "";
 position: absolute;
 transition:
 background-color .3s var(--n-bezier),
 left .3s var(--n-bezier),
 opacity .3s var(--n-bezier),
 max-width .3s var(--n-bezier),
 box-shadow .3s var(--n-bezier);
 `)]),s("active",[i("rail","background-color: var(--n-rail-color-active);")]),s("loading",[i("rail",`
 cursor: wait;
 `)]),s("disabled",[i("rail",`
 cursor: not-allowed;
 opacity: .5;
 `)])]);const Ve=["aria-checked","tabindex","onClick","onFocus","onBlur","onKeyup","onKeydown"],Fe={...Z.props,size:String,value:{type:[String,Number,Boolean],default:void 0},loading:Boolean,defaultValue:{type:[String,Number,Boolean],default:!1},disabled:{type:Boolean,default:void 0},round:{type:Boolean,default:!0},"onUpdate:value":[Function,Array],onUpdateValue:[Function,Array],checkedValue:{type:[String,Number,Boolean],default:!0},uncheckedValue:{type:[String,Number,Boolean],default:!1},railStyle:Function,rubberBand:{type:Boolean,default:!0},spinProps:Object,onChange:[Function,Array]};let F;var Pe=me({name:"Switch",props:Fe,slots:Object,setup(e){F===void 0&&(typeof CSS<"u"?typeof CSS.supports<"u"?F=CSS.supports("width","max(1px)"):F=!1:F=!0);const{mergedClsPrefixRef:c,inlineThemeDisabled:f,mergedComponentPropsRef:o}=pe(e),v=Z("Switch","-switch",Re,ze,e,c),g=Ce(e,{mergedSize(t){var x,y;if(e.size!==void 0)return e.size;if(t)return t.mergedSize.value;const w=(y=(x=o==null?void 0:o.value)==null?void 0:x.Switch)==null?void 0:y.size;return w||"medium"}}),{mergedSizeRef:$,mergedDisabledRef:m}=g,z=E(e.defaultValue),W=Be(e,"value"),p=we(W,z),M=K(()=>p.value===e.checkedValue),a=E(!1),n=E(!1),R=K(()=>{const{railStyle:t}=e;if(t)return t({focused:n.value,checked:M.value})});function P(t){const{"onUpdate:value":w,onChange:x,onUpdateValue:y}=e,{nTriggerFormInput:T,nTriggerFormChange:L}=g;w&&N(w,t),y&&N(y,t),x&&N(x,t),z.value=t,T(),L()}function Q(){const{nTriggerFormFocus:t}=g;t()}function ee(){const{nTriggerFormBlur:t}=g;t()}function te(){e.loading||m.value||(p.value!==e.checkedValue?P(e.checkedValue):P(e.uncheckedValue))}function ie(){n.value=!0,Q()}function ae(){n.value=!1,ee(),a.value=!1}function oe(t){e.loading||m.value||t.key===" "&&(p.value!==e.checkedValue?P(e.checkedValue):P(e.uncheckedValue),a.value=!1)}function re(t){e.loading||m.value||t.key===" "&&(t.preventDefault(),a.value=!0)}const X=K(()=>{const{value:t}=$,{self:{opacityDisabled:w,railColor:x,railColorActive:y,buttonBoxShadow:T,buttonColor:L,boxShadowFocus:ne,loadingColor:le,textColor:se,iconColor:de,[S("buttonHeight",t)]:u,[S("buttonWidth",t)]:ce,[S("buttonWidthPressed",t)]:ue,[S("railHeight",t)]:h,[S("railWidth",t)]:V,[S("railBorderRadius",t)]:he,[S("buttonBorderRadius",t)]:be},common:{cubicBezierEaseInOut:fe}}=v.value;let D,H,U;return F?(D=`calc((${h} - ${u}) / 2)`,H=`max(${h}, ${u})`,U=`max(${V}, calc(${V} + ${u} - ${h}))`):(D=O((d(h)-d(u))/2),H=O(Math.max(d(h),d(u))),U=d(h)>d(u)?V:O(d(V)+d(u)-d(h))),{"--n-bezier":fe,"--n-button-border-radius":be,"--n-button-box-shadow":T,"--n-button-color":L,"--n-button-width":ce,"--n-button-width-pressed":ue,"--n-button-height":u,"--n-height":H,"--n-offset":D,"--n-opacity-disabled":w,"--n-rail-border-radius":he,"--n-rail-color":x,"--n-rail-color-active":y,"--n-rail-height":h,"--n-rail-width":V,"--n-width":U,"--n-box-shadow-focus":ne,"--n-loading-color":le,"--n-text-color":se,"--n-icon-color":de}}),B=f?xe("switch",K(()=>$.value[0]),X,e):void 0;return{handleClick:te,handleBlur:ae,handleFocus:ie,handleKeyup:oe,handleKeydown:re,mergedRailStyle:R,pressed:a,mergedClsPrefix:c,mergedValue:p,checked:M,mergedDisabled:m,cssVars:f?void 0:X,themeClass:B==null?void 0:B.themeClass,onRender:B==null?void 0:B.onRender}},render(){const{mergedClsPrefix:e,mergedDisabled:c,checked:f,mergedRailStyle:o,onRender:v,$slots:g}=this;v==null||v();const{checked:$,unchecked:m,icon:z,"checked-icon":W,"unchecked-icon":p}=g,M=!(A(z)&&A(W)&&A(p));return b(),C("div",{role:"switch","aria-checked":f,class:r([`${e}-switch`,this.themeClass,M&&`${e}-switch--icon`,f&&`${e}-switch--active`,c&&`${e}-switch--disabled`,this.round&&`${e}-switch--round`,this.loading&&`${e}-switch--loading`,this.pressed&&`${e}-switch--pressed`,this.rubberBand&&`${e}-switch--rubber-band`]),tabindex:this.mergedDisabled?void 0:0,style:J(this.cssVars),onClick:this.handleClick,onFocus:this.handleFocus,onBlur:this.handleBlur,onKeyup:this.handleKeyup,onKeydown:this.handleKeydown},[_("div",{class:r(`${e}-switch__rail`),"aria-hidden":"true",style:J(o)},[l(()=>k($,a=>k(m,n=>a||n?(b(),C("div",{key:4,"aria-hidden":!0,class:r(`${e}-switch__children-placeholder`)},[_("div",{class:r(`${e}-switch__rail-placeholder`)},[_("div",{class:r(`${e}-switch__button-placeholder`)},null,2),l(()=>a)],2),_("div",{class:r(`${e}-switch__rail-placeholder`)},[_("div",{class:r(`${e}-switch__button-placeholder`)},null,2),l(()=>n)],2)],2)):null))),_("div",{class:r(`${e}-switch__button`)},[l(()=>k(z,a=>k(W,n=>k(p,R=>(b(),G(Se,null,{default:()=>this.loading?(b(),G(ye,ke({key:"loading",clsPrefix:e,strokeWidth:20},this.spinProps),null,16,["clsPrefix"])):this.checked&&(n||a)?(b(),C("div",{class:r(`${e}-switch__button-icon`),key:n?"checked-icon":"icon"},[l(()=>n||a)],2)):!this.checked&&(R||a)?(b(),C("div",{class:r(`${e}-switch__button-icon`),key:R?"unchecked-icon":"icon"},[l(()=>R||a)],2)):null},1024)))))),l(()=>k($,a=>a&&(b(),C("div",{key:"checked",class:r(`${e}-switch__checked`)},[l(()=>a)],2)))),l(()=>k(m,a=>a&&(b(),C("div",{key:"unchecked",class:r(`${e}-switch__unchecked`)},[l(()=>a)],2))))],2)],6)],46,Ve)}});export{Pe as S};
