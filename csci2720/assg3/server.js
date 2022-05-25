/*
  CSCI2720 Assignment 3
  Ng Wing Yin 1155127101
*/

// Require
const bodyParser = require('body-parser');
const cors = require('cors');
const express = require('express');
const mongoose = require('mongoose');

// Create express app
const app = express();
app.use(cors());
app.use(express.json());
app.use(bodyParser.urlencoded({ extended: false }));

// Connect database
mongoose.connect('mongodb+srv://stu074:p984744-@csci2720.6hfif.mongodb.net/stu074');
const db = mongoose.connection;
db.on('error', console.error.bind(console, 'Connection error:'));
db.once('open', function () {
  console.log('Connection is open...');

  // Schemas
  const LocationSchema = mongoose.Schema({
    locId: { type: Number, required: true, unique: true },
    name: { type: String, required: true },
    quota: { type: Number },
  });
  const Location = mongoose.model('Location', LocationSchema);

  const EventSchema = mongoose.Schema({
    eventId: { type: Number, required: true, unique: true },
    name: { type: String, required: true },
    location: { type: Object, required: true },
    quota: { type: Number },
  });
  const Event = mongoose.model('Event', EventSchema);

  // Create location
  app.post('/loc', (req, res) => {
    const { body: { name, quota } } = req;
    Location.find().sort({ locId: 'descending' }).then((data) => {
      const newLocId = data && data[0] ? data[0].locId + 1 : 0;
      Location.create({ locId: newLocId, name: name, quota: +quota }, (err, e) => {
        if (err) { return res.send(err); }
        res.status(201).send(e);
      });
    });
  });

  // Q1
  app.get('/event/:eventId', (req, res) => {
    const { params: { eventId } } = req;
    Event.findOne({ eventId }, 'eventId name location quota', (err, e) => {
      if (err) { return res.send(err); }
      if (!e) { return res.status(404).send('Event ID not found'); }
      res.send({ eventId, name: e.name, loc: { locId: e.location.locId, name: e.location.name }, quota: e.quota });
    });
  });

  // Q2
  app.post('/event', (req, res) => {
    const { body: { name, location, quota } } = req;
    Event.find().sort({ eventId: 'descending' }).then((data) => {
      const newEventId = data && data[0] ? data[0].eventId + 1 : 0;
      Location.findOne({ locId: location }, 'locId name quota', (errLocFind, eLocFind) => {
        if (errLocFind) { return res.send(errLocFind); }
        if (!eLocFind) { return res.status(404).send('Location ID not found'); }
        if (quota > eLocFind.quota) { return res.send('Quota exceeded'); }
        Event.create({ eventId: newEventId, name, location: eLocFind, quota }, (errEvCreate, eEvCreate) => {
          if (errEvCreate) { return res.send(errEvCreate); }
          res.status(201).header({ Location: `/event/${newEventId}` }).send(eEvCreate);
        });
      });
    });
  });

  // Q3
  app.delete('/event/:eventId', (req, res) => {
    const { params: { eventId } } = req;
    Event.findOneAndRemove({ eventId }, (err, e) => {
      if (err) { return res.send(err); }
      if (!e) { return res.status(404).send('Event ID not found'); }
      res.status(204).send();
    });
  });

  // Q4
  app.get('/event', (req, res) => {
    Event.find().sort({ eventId: 'ascending' }).then((data) => {
      res.send(data.map((item) => ({ eventId: item.eventId, name: item.name, loc: { locId: item.location.locId, name: item.location.name }, quota: item.quota })));
    });
  });

  // Q5
  app.get('/loc/:locationId', (req, res) => {
    const { params: { locationId } } = req;
    Location.findOne({ locationId }, 'locationId name location quota', (err, e) => {
      if (err) { return res.send(err); }
      if (!e) { return res.status(404).send('Location ID not found'); }
      res.send({ locId: e.locId, name: e.name, quota: e.quota });
    });
  });

  // Q6, Q7
  app.get('/loc', (req, res) => {
    const { query: { quota } } = req;
    Location.find({ quota: { $gte: quota ? quota : 0 } }).sort({ locId: 'ascending' }).then((data) => {
      res.send(data.map((item) => ({ locId: item.locId, name: item.name, quota: item.quota })));
    });
  });

  // Q8
  app.put('/event/:eventId',  (req, res) => {
    const { params: { eventId }, body: { name, location, quota } } = req;
    Event.findOne({ eventId }, 'eventId', (err, e) => {
      if (err) { return res.send(err); }
      if (!e) { return res.status(404).send('Event ID not found'); }

      const newName = name === '' ? e.name : name;
      const newLocation = location === '' ? e.location : location;
      const newQuota = quota === '' ? e.quota : quota;

      Location.findOne({ locId: newLocation }, 'locId name quota', (errLocFind, eLocFind) => {
        if (errLocFind) { return res.send(errLocFind); }
        if (!eLocFind) { return res.status(404).send('Location ID not found'); }
        if (quota > eLocFind.quota) { return res.send('Quota exceeded'); }

        Event.findOneAndUpdate({ eventId: eventId }, { name: newName, location: eLocFind, quota: newQuota }, { new: true }, (errEvUpdate, eEvUpdate) => {
          if (errEvUpdate) { return res.send(errEvUpdate); }
          res.send({ eventId, name: eEvUpdate.name, loc: { locId: eLocFind.locId, name: eLocFind.name }, quota: eEvUpdate.quota });
        });
      });
    });
  });
});

const server = app.listen(3000);

