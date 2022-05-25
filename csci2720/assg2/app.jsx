/*
  CSCI2720 Assigment 2
  Ng Wing Yin 1155127101
*/

// Setup router
const { useState, useEffect } = React;
const { BrowserRouter, Routes, Route, Link, useMatch, useParams, useLocation } = ReactRouterDOM;

// Hardcoded data
const data = [
  { filename: "cuhk-2013.jpg", year: 2013, remarks: "Sunset over CUHK" },
  { filename: "cuhk-2017.jpg", year: 2017, remarks: "Bird's-eye view of CUHK" },
  { filename: "sci-2013.jpg", year: 2013, remarks: "The CUHK Emblem" },
  { filename: "shb-2013.jpg", year: 2013, remarks: "The Engineering Buildings" },
  { filename: "stream-2009.jpg", year: 2009, remarks: "Nature hidden in the campus" },
];

// Title component
function Title({ name }) {
  return (
    <header className="bg-warning">
      <h1 className="display-4 text-center">{name}</h1>
    </header>
  );
}

// Long link component
function LongLink({ label, to }) {
  const match = useMatch({ path: to });
  return (
    <li className={match ? 'active' : ''}>
      { match && '> ' }
      <Link to={to}>{label}</Link>
    </li>
  );
}

// File card component
function FileCard({ i }) {
  const [selected, setSelected] = useState(-1);
  const [hovered, setHovered] = useState(-1);

  const handleClick = (index, e) => {
    setSelected(selected != index ? index : -1);
  }

  const handleMouseOver = (index, e) => {
    setHovered(index);
  }

  const handleMouseOut = (e) => {
    setHovered(-1);
  }

  return (
    <div className="card d-inline-block m-2" style={{ width: selected == i ? '100%' : hovered == i ? 220 : 200 }} onClick={(e) => handleClick(i, e)} onMouseOver={(e) => handleMouseOver(i, e)} onMouseOut={(e) => handleMouseOut(e)}>
      <img src={'images/' + data[i].filename} className="w-100" />
      <div className="card-body">
        <h6 className="card-title">{data[i].filename}</h6>
        <p className="card-text">{data[i].year}</p>
        { selected == i && <p className="card-text">{data[i].remarks}</p> }
      </div>
    </div>
  );
}

// Home component
function Home() {
  return (
    <main className="container">
      <img src={'diagram.png'} className="h-100" />
    </main>
  );
}

// Gallery component
function Gallery() {
  return (
    <main className="container">
      { data.map((file, index) => <FileCard i={index} key={index} />) }
    </main>
  );
}

// Slideshow component
function Slideshow() {
  const [currentImageID, setCurrentImageID] = useState(0);
  const [currentImageInterval, setCurrentImageInterval] = useState(1500);
  const [timeoutID, setTimeoutID] = useState();

  useEffect(() => {
    clearTimeout(timeoutID);
    handleStart();
  }, [currentImageID]);

  const handleStart = (e) => {
    setTimeoutID(setTimeout(() => {
      setCurrentImageID((currentImageID + 1) % data.length);
    }, currentImageInterval));
  };

  const handleStop = (e) => {
    clearTimeout(timeoutID);
  };

  const handleFaster = (e) => {
    setCurrentImageInterval(Math.max(currentImageInterval - 250, 250));
  };

  const handleSlower = (e) => {
    setCurrentImageInterval(currentImageInterval + 250);
  };

  return (
    <main className="container">
      <button onClick={(e) => handleStart(e)}>Start slideshow</button>
      <button onClick={(e) => handleStop(e)}>Stop slideshow</button>
      <button onClick={(e) => handleFaster(e)}>Faster</button>
      <button onClick={(e) => handleSlower(e)}>Slower</button>
      <img src={'images/' + data[currentImageID].filename} className="w-100" />
      <h6>{data[currentImageID].filename}</h6>
    </main>
  );
}

// No match component
function NoMatch() {
  const loc = useLocation();
  return (
    <div>
      <h3>No match for <code>{loc.pathname}</code></h3>
    </div>
  );
}

// App component
function App({ name }) {
  return (
    <React.StrictMode>
      <Title name={name} />
      <BrowserRouter>
        <ul>
          <LongLink to="/" label="Home" />
          <LongLink to="/gallery" label="Gallery" />
          <LongLink to="/slideshow" label="Slideshow" />
        </ul>

        <Routes>
          <Route path="/" element={<Home />} />
          <Route path="/gallery" element={<Gallery />} />
          <Route path="/slideshow" element={<Slideshow />} />
          <Route path="*" element={<NoMatch />} />
        </Routes>
      </BrowserRouter>
    </React.StrictMode>
  );
}

// Render
ReactDOM.render(<App name="CUHK Pictures" />, document.querySelector('#app'));