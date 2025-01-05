<?php

class newsReader{
	
	private $xml;
	private $perpage;
	private $news;
	private $topic;
	
	public function setString($string,$perpage=0)
	{
		$this->xml = $string;
		$this->perpage = $perpage;
	}
	
	public function setTopic($topic)
	{
		$this->topic=$topic;
	}
	
	public function print_all()
	{
		$xml = simplexml_load_string($this->xml);
		$json = json_encode($xml);
		$this->news = json_decode($json,TRUE);
		$this->news = $this->news['news'];
		
		if($this->topic!=null) $resp="#$this->topic<br/><br/>";
		else $resp="";
		
		foreach($this->news AS $key => $news)
		{
			if($this->topic=="" or $news['target']==$this->topic)
			$resp.="<p class='newsblock'><span class='newsdate'>{$news['date']}</span><br/><span class='newstext'>{$news['text']}</span></p>";
		}
		
		return $resp;
	}
	
	public function print_page()
	{
		$xml = simplexml_load_string($this->xml);
		$json = json_encode($xml);
		$this->news = json_decode($json,TRUE);
		$this->news = $this->news['news'];
		
		$resp="";
			
		for($c=0;$c<$this->perpage;$c++)
		{
			if(!$this->news[$c]) break;
			
			//if(strlen($this->news[$c]['text'])>NEWS_LENGTH) $this->news[$c]['text']=substr($this->news[$c]['text'],0,NEWS_LENGTH)."...";
			
			if($this->news[$c]['topic']!=$this->topic && $this->news[$c]['topic']=="main") continue;
			
			$resp.="<a href='javascript:void(0);' onclick=\"setpage('".$this->news[$c]['target']."');\">
					<p class='newsblock'>";
					//appendnews('<b>".$this->news[$c]['date'].":</b> ".$this->news[$c]['text']."')
					
			if($this->news[$c]['icon']!="")
			{
				$resp.="<span style='float:left; margin-bottom:10px;'><img src='img/{$this->news[$c]['icon']}.png' width='30px' height='30px' alt='important' /></span>";
			}	
					
			$resp.="<span class='newsdate'>{$this->news[$c]['date']}</span> - <span class='newstext'>".$this->news[$c]['text']." >></span></p>
					</a>";
					
			
		}
		
		
		return $resp;
		
	}
		
}

?>